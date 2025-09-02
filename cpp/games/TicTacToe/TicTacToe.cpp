#include "TicTacToe.h"
#include <algorithm>
#include <stdexcept>

TicTacToe::TicTacToe() = default;
TicTacToe::~TicTacToe() = default;

GameState TicTacToe::start() {
    auto* initialState = new std::array<std::array<int, 3>, 3>();
    for (auto& row : *initialState) {
        row.fill(0);
    }
    return GameState(initialState, false);
}

bool TicTacToe::checkWinner(const std::array<std::array<int, 3>, 3>& state) {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if ((state[i][0] == 1 && state[i][1] == 1 && state[i][2] == 1) ||
            (state[0][i] == 1 && state[1][i] == 1 && state[2][i] == 1)) {
            return true;
        }
    }
    
    // Check diagonals
    if ((state[0][0] == 1 && state[1][1] == 1 && state[2][2] == 1) ||
        (state[0][2] == 1 && state[1][1] == 1 && state[2][0] == 1)) {
        return true;
    }
    
    return false;
}

std::pair<GameState, float> TicTacToe::move(const GameState& state, int action) {
    if (!isValidAction(state, action)) {
        throw std::invalid_argument("Invalid action");
    }

    auto* currentState = static_cast<std::array<std::array<int, 3>, 3>*>(state.state);
    auto* newState = new std::array<std::array<int, 3>, 3>(*currentState);
    
    int row = action / 3;
    int col = action % 3;
    (*newState)[row][col] = 1;

    bool isTerminal = checkWinner(*newState);
    float reward = isTerminal? 1:0;
    if (!isTerminal) {
        // Check if board is full
        isTerminal = true;
        for (const auto& row : *newState) {
            for (int cell : row) {
                if (cell == 0) {
                    isTerminal = false;
                    break;
                }
            }
            if (!isTerminal) break;
        }
    }

    return std::make_pair(GameState(newState, isTerminal), reward);
}

void TicTacToe::setState(GameState& state, int player) {
    auto* board = static_cast<std::array<std::array<int, 3>, 3>*>(state.state);
    if (player == -1) {
        for (auto& row : *board) {
            for (int& cell : row) {
                cell = -cell;
            }
        }
    }
}

GameState TicTacToe::flipBoard(const GameState& state) {
    auto* currentState = static_cast<std::array<std::array<int, 3>, 3>*>(state.state);
    auto* newState = new std::array<std::array<int, 3>, 3>();
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            (*newState)[2-i][2-j] = -(*currentState)[i][j];
        }
    }
    
    return GameState(newState, state.isTerminal);
}

bool TicTacToe::isValidAction(const GameState& state, int action) {
    if (state.isTerminal) return false;
    
    auto* board = static_cast<std::array<std::array<int, 3>, 3>*>(state.state);
    int row = action / 3;
    int col = action % 3;
    return (*board)[row][col] == 0;
}

std::vector<int> TicTacToe::getValidActions(const GameState& state) {
    std::vector<int> validActions;
    for (int action = 0; action < 9; action++) {
        if (isValidAction(state, action)) {
            validActions.push_back(action);
        }
    }
    return validActions;
}

float TicTacToe::getOpponentReward(float reward) {
    return -reward;
}

std::vector<float> TicTacToe::encodeState(const GameState& state) {
    auto* board = static_cast<std::array<std::array<int, 3>, 3>*>(state.state);
    std::vector<float> encoded;
    encoded.reserve(9);
    
    for (const auto& row : *board) {
        for (int cell : row) {
            encoded.push_back(static_cast<float>(cell));
        }
    }
    
    return encoded;
}

int TicTacToe::actionSpaceSize() {
    return 9;
}

int TicTacToe::stateSpaceSize() {
    return 9;
}