#include "mcts.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <numeric>
#include <cassert>

// Static member initialization
int MCTSNode::nextNodeId = 0;

MCTSNode::MCTSNode(
    Game* game,
    const GameState& state,
    int actionTaken,
    int player,
    float reward,
    MCTSNode* parent,
    float probPrior,
    float explorationWeight,
    int nodeId
) : game(game), state(state), actionTaken(actionTaken), player(player), 
    reward(reward), parent(parent), probPrior(probPrior), 
    explorationWeight(explorationWeight), valueSum(0.0f), visits(0),
    nodeId(nodeId == 0 ? ++nextNodeId : nodeId) {
}

MCTSNode::~MCTSNode() {
    // Unique pointers will automatically clean up children
}

bool MCTSNode::isFullyExpanded() const {
    return !children.empty() || (state.isTerminal && visits > 0);
}

float MCTSNode::getUCB(const MCTSNode* child) const {
    float qValue = 0.0f;
    if (child->visits > 0) {
        qValue = -child->valueSum / child->visits;
    }
    
    return qValue + explorationWeight * child->probPrior * 
           (std::sqrt(static_cast<float>(visits)) / (1.0f + child->visits));
}

MCTSNode* MCTSNode::bestChild() const {
    if (children.empty()) {
        return nullptr;
    }
    
    auto bestIt = std::max_element(children.begin(), children.end(),
        [this](const std::unique_ptr<MCTSNode>& a, const std::unique_ptr<MCTSNode>& b) {
            return getUCB(a.get()) < getUCB(b.get());
        });
    
    return bestIt->get();
}

void MCTSNode::expand(const std::vector<float>& policy) {
    std::vector<int> validActions = game->getValidActions(state);
    
    for (int action : validActions) {
        auto [newState, r] = game->move(state, action);
        newState = game->flipBoard(newState);
        int newPlayer = -player;
        float newReward = game->getOpponentReward(r);
        
        auto childNode = std::make_unique<MCTSNode>(
            game, newState, action, newPlayer, newReward,
            this, policy[action], explorationWeight, 0
        );
        
        children.push_back(std::move(childNode));
    }
}

void MCTSNode::backpropagate(float value) {
    visits++;
    valueSum += value;
    
    if (parent != nullptr) {
        parent->backpropagate(game->getOpponentReward(value));
    }
}

void MCTSNode::print(int depth) const {
    std::string indent(depth * 2, ' ');
    std::string playerStr = (player == 1) ? "AI" : "H";
    std::cout << indent << playerStr << "(id=" << nodeId 
              << ", action_taken=" << actionTaken 
              << ", value_sum=" << valueSum 
              << ", visits=" << visits 
              << ", reward=" << reward << ")" << std::endl;
    
    for (const auto& child : children) {
        child->print(depth + 1);
    }
}

MCTS::MCTS(Game* game, Model* model, int numSimulations, float explorationWeight)
    : game(game), model(model), numSimulations(numSimulations), 
      explorationWeight(explorationWeight) {
}

std::vector<float> MCTS::search(const GameState& state) {
    auto root = std::make_unique<MCTSNode>(game, state, -1, 1, 0.0f, nullptr, 1.0f, explorationWeight);
    
    for (int i = 0; i < numSimulations; ++i) {
        MCTSNode* parent = root.get();
        
        // Selection phase
        while (parent->isFullyExpanded()) {
            if (parent->state.isTerminal) {
                break;
            }
            parent = parent->bestChild();
        }
        
        float value;
        
        if (!parent->state.isTerminal) {
            // Expansion and evaluation phase
            std::vector<float> encodedState = game->encodeState(parent->state);
            auto [policy, predictedValue] = model->predict(encodedState);
            
            // Apply validity mask to policy
            std::vector<int> validActions = game->getValidActions(parent->state);
            std::vector<float> validity(game->actionSpaceSize(), 0.0f);
            for (int action : validActions) {
                validity[action] = 1.0f;
            }
            
            // Element-wise multiplication and normalization
            float policySum = 0.0f;
            for (size_t j = 0; j < policy.size(); ++j) {
                policy[j] *= validity[j];
                policySum += policy[j];
            }
            
            if (policySum > 0.0f) {
                for (float& p : policy) {
                    p /= policySum;
                }
            }
            
            parent->expand(policy);
            value = predictedValue;
        } else {
            value = parent->reward;
        }
        
        // Backpropagation phase
        parent->backpropagate(value);
    }
    
    // Calculate action probabilities based on visit counts
    std::vector<float> probs(game->actionSpaceSize(), 0.0f);
    float totalVisits = 0.0f;
    
    for (const auto& child : root->children) {
        probs[child->actionTaken] = static_cast<float>(child->visits);
        totalVisits += child->visits;
    }

    std::cout << "unnormalized probs: ";
    for (float prob : probs) {
        std::cout << prob << " ";
    }
    std::cout << std::endl;

    // Normalize probabilities
    if (totalVisits > 0.0f) {
        for (float& prob : probs) {
            prob /= totalVisits;
        }
    }
    
    return probs;
}

// RandomModel implementation
RandomModel::RandomModel(int stateSize, int actionSize)
    : stateSize(stateSize), actionSize(actionSize) {
}

std::pair<std::vector<float>, float> RandomModel::predict(const std::vector<float>& /* encodedState */) {
    std::vector<float> policy(actionSize, 1.0f / actionSize);
    return std::make_pair(policy, 0.0f);
}
