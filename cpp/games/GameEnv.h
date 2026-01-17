#ifndef GAME_ENV_H
#define GAME_ENV_H

#include <vector>
#include <memory>

class GameState {
public:
    GameState(void* state = nullptr, bool isTerminal = false);

    void* state;
    bool isTerminal;
};

class Game {
public:
    Game();
    virtual ~Game();

    virtual GameState start() = 0;
    virtual std::pair<GameState, float> move(const GameState& state, T action) = 0;
    virtual void setState(GameState& state, int player) = 0;
    virtual GameState flipBoard(const GameState& state) = 0;
    virtual bool isValidAction(const GameState& state, int action) = 0;
    virtual std::vector<T> getValidActions(const GameState& state) = 0;
    virtual float getOpponentReward(float reward) = 0;
    virtual std::vector<float> encodeState(const GameState& state) = 0;
    virtual int actionSpaceSize() = 0;
    virtual int stateSpaceSize() = 0;
    // Helper method to display the board
    virtual void displayBoard(const GameState& state) const = 0;
    virtual bool checkEq(const GameState& lhs, const GameState& rhs) const = 0;
};

#endif // GAME_ENV_H