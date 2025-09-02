#include "algorithms/mcts.h"
#include "games/ConnectFour/ConnectFour.h"
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>

int main() {
    const int numGames = 2;
    int bot1Wins = 0, bot2Wins = 0, draws = 0;
    long long bot1TotalTime = 0;
    long long bot2TotalTime = 0;

    for (int gameIdx = 0; gameIdx < numGames; ++gameIdx) {
        auto model1 = std::make_unique<RandomModel>(42, 7);
        auto model2 = std::make_unique<RandomModel>(42, 7);
        auto game = std::make_unique<ConnectFour>();
        MCTS mcts1(game.get(), model1.get(), 12700, 1.0f);
        MCTS2 mcts2(game.get(), model2.get(), 10000, 1.0f);

        GameState state = game->start();
        bool bot1Turn = (gameIdx % 2 == 0); // Alternate starting player
        float result = 0;

        while (!state.isTerminal) {
            int action = 0;
            if (bot1Turn) {
                auto start = std::chrono::high_resolution_clock::now();
                std::vector<float> probs = mcts1.search(state);
                auto end = std::chrono::high_resolution_clock::now();
                bot1TotalTime += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                // Pick best action
                int bestAction = 0;
                float bestProb = probs[0];
                for (size_t i = 1; i < probs.size(); ++i) {
                    if (probs[i] > bestProb) {
                        bestProb = probs[i];
                        bestAction = static_cast<int>(i);
                    }
                }
                action = bestAction;
            } else {
                auto start = std::chrono::high_resolution_clock::now();
                std::vector<float> probs = mcts2.search(state);
                auto end = std::chrono::high_resolution_clock::now();
                bot2TotalTime += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                int bestAction = 0;
                float bestProb = probs[0];
                for (size_t i = 1; i < probs.size(); ++i) {
                    if (probs[i] > bestProb) {
                        bestProb = probs[i];
                        bestAction = static_cast<int>(i);
                    }
                }
                action = bestAction;
            }
            auto [nextState, reward] = game->move(state, action);
            state = game->flipBoard(nextState);
            bot1Turn = !bot1Turn;
            result = reward;

            // if (bot1Turn) {
            //     game->displayBoard(state);
            // } else {
            //     game->displayBoard(nextState);
            // }
        }

        // Game ended, check result
        if (!bot1Turn) { // bot1 ended the game
            if (result > 0) bot1Wins++;
            else if (result < 0) bot2Wins++;
            else draws++;
        } else { // bot2 ended the game
            if (result > 0) bot2Wins++;
            else if (result < 0) bot1Wins++;
            else draws++;
        }
    }

    std::cout << "After " << numGames << " games between two MCTS bots:" << std::endl;
    std::cout << "Bot1 wins: " << bot1Wins << std::endl;
    std::cout << "Bot2 wins: " << bot2Wins << std::endl;
    std::cout << "Draws: " << draws << std::endl;
    std::cout << "Bot1 total thinking time: " << bot1TotalTime << " ms" << std::endl;
    std::cout << "Bot2 total thinking time: " << bot2TotalTime << " ms" << std::endl;
    return 0;
}
