#ifndef CONNECTFOUR_H
#define CONNECTFOUR_H

#include "../GameEnv.h"
#include <array>

class ConnectFour : public Game {
public:
    ConnectFour();
    ~ConnectFour() override;

    GameState start() override;
    GameState move(const GameState& state, int action) override;
    void setState(GameState& state, int player) override;
    GameState flipBoard(const GameState& state) override;
    bool isValidAction(const GameState& state, int action) override;
    std::vector<int> getValidActions(const GameState& state) override;
    float getOpponentReward(float reward) override;
    std::vector<float> encodeState(const GameState& state) override;
    int actionSpaceSize() override;
    int stateSpaceSize() override;

private:
    bool checkWinner(const std::array<std::array<int, 7>, 6>& state);
    static const int ROWS = 6;
    static const int COLS = 7;
};

#endif // CONNECTFOUR_H