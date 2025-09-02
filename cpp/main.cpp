#include "algorithms/mcts.h"
#include "games/ConnectFour/ConnectFour.h"
#include "games/TicTacToe/TicTacToe.h"
#include <iostream>
#include <memory>

int main() {
    // Create a random model and ConnectFour game
    auto model = std::make_unique<RandomModel>(42, 7); // ConnectFour state/action sizes
    auto game = std::make_unique<ConnectFour>();
    
    // Initialize MCTS
    MCTS mcts(game.get(), model.get(), 1000, 1.0f);
    
    // Start the game
    GameState state = game->start(); // From AI's perspective
    
    std::cout << "ConnectFour Game with MCTS AI" << std::endl;
    std::cout << "Enter column numbers 0-6 for your moves" << std::endl;
    
    while (!state.isTerminal) {
        // Human move first
        GameState humanState = game->flipBoard(state);
        
        // Display the board
        std::cout << "\nCurrent board:" << std::endl;
        game->displayBoard(humanState);
        
        int action;
        while (true) {
            std::cout << "Enter your move (0-6): ";
            std::cin >> action;
            
            if (!game->isValidAction(humanState, action)) {
                std::cout << "Invalid move. Try again." << std::endl;
                continue;
            }
            break;
        }
        
        // Apply human move
        auto [newState, r] = game->move(humanState, action);
        state = game->flipBoard(newState); // Convert back to AI perspective
        
        if (state.isTerminal) {
            std::cout << "\nFinal board:" << std::endl;
            game->displayBoard(game->flipBoard(state));
            std::cout << "Game over!" << std::endl;
            break;
        }

        std::cout << "\nAI's perspective board:" << std::endl;
        game->displayBoard(state);
        
        // AI move
        std::cout << "\nAI is thinking..." << std::endl;
        std::vector<float> probs = mcts.search(state);

        // print probs
        std::cout << "AI action probabilities: ";
        for (size_t i = 0; i < probs.size(); ++i) {
            std::cout << "Action " << i << ": " << probs[i] << " ";
        }
        std::cout << std::endl;
        
        // Find the action with highest probability
        int bestAction = 0;
        float bestProb = probs[0];
        for (size_t i = 1; i < probs.size(); ++i) {
            if (probs[i] > bestProb) {
                bestProb = probs[i];
                bestAction = static_cast<int>(i);
            }
        }
        
        std::cout << "AI chooses column: " << bestAction << std::endl;
        
        // Apply AI move
        auto [nextState, _] = game->move(state, bestAction);
        state = nextState;
        
        if (state.isTerminal) {
            std::cout << "\nFinal board:" << std::endl;
            game->displayBoard(game->flipBoard(state));
            std::cout << "Game over!" << std::endl;
            break;
        }
    }
    
    return 0;
}
