#ifndef CONNECTFOUR_H
#define CONNECTFOUR_H

#include "../GameEnv.h"
#include <array>

class ConnectFour : public Game<int> {
public:
    ConnectFour();
    ~ConnectFour() override;

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
    
    // Helper method to display the board
    void displayBoard(const GameState& state) const;

    bool checkEq(const GameState& lhs, const GameState& rhs) const;

private:
    bool checkWinner(const std::array<std::array<int, 7>, 6>& state);
    static const int ROWS = 6;
    static const int COLS = 7;
};

#endif // CONNECTFOUR_H