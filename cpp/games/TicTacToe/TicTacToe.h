#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "../GameEnv.h"
#include <array>

class TicTacToe : public Game {
public:
    TicTacToe();
    ~TicTacToe() override;

    GameState start() override;
    std::pair<GameState, float> move(const GameState& state, int action) override;
    void setState(GameState& state, int player) override;
    GameState flipBoard(const GameState& state) override;
    bool isValidAction(const GameState& state, int action) override;
    std::vector<int> getValidActions(const GameState& state) override;
    float getOpponentReward(float reward) override;
    std::vector<float> encodeState(const GameState& state) override;
    int actionSpaceSize() override;
    int stateSpaceSize() override;

private:
    bool checkWinner(const std::array<std::array<int, 3>, 3>& state);
};

#endif // TICTACTOE_H