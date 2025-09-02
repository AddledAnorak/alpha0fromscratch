#ifndef MCTS_H
#define MCTS_H

#include "../games/GameEnv.h"
#include <vector>
#include <memory>
#include <cmath>

// Forward declaration for neural network model interface
class Model {
public:
    virtual ~Model() = default;
    virtual std::pair<std::vector<float>, float> predict(const std::vector<float>& encodedState) = 0;
};

class MCTSNode {
public:
    MCTSNode(
        Game* game,
        const GameState& state,
        int actionTaken = -1,
        int player = 1,
        float reward = 0.0f,
        MCTSNode* parent = nullptr,
        float probPrior = 1.0f,
        float explorationWeight = 1.0f,
        int nodeId = 0
    );

    ~MCTSNode();

    bool isFullyExpanded() const;
    float getUCB(const MCTSNode* child) const;
    MCTSNode* bestChild() const;
    void expand(const std::vector<float>& policy);
    void backpropagate(float value);
    void print(int depth = 0) const;

    // Public members for easier access (similar to Python version)
    // Order matches constructor initialization list
    Game* game;
    GameState state;
    int actionTaken;
    int player;
    float reward;
    MCTSNode* parent;
    float probPrior;
    float explorationWeight;
    float valueSum;
    int visits;
    int nodeId;
    
    std::vector<std::unique_ptr<MCTSNode>> children;

private:
    static int nextNodeId;
};

class MCTS {
public:
    MCTS(Game* game, Model* model, int numSimulations = 1000, float explorationWeight = 1.0f);
    ~MCTS() = default;

    std::vector<float> search(const GameState& state);

private:
    Game* game;
    Model* model;
    int numSimulations;
    float explorationWeight;
};

class MCTS2 {
public:
    MCTS2(Game* game, Model* model, int numSimulations = 1000, float explorationWeight = 1.0f);
    ~MCTS2() = default;

    std::vector<float> search(const GameState& state);

private:
    Game* game;
    Model* model;
    std::unique_ptr<MCTSNode> root;
    int numSimulations;
    float explorationWeight;
};

// Simple random model implementation for testing
class RandomModel : public Model {
public:
    RandomModel(int stateSize, int actionSize);
    std::pair<std::vector<float>, float> predict(const std::vector<float>& encodedState) override;

private:
    int stateSize;
    int actionSize;
};

#endif // MCTS_H
