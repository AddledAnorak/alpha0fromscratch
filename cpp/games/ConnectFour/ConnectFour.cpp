#include "ConnectFour.h"
#include <stdexcept>

ConnectFour::ConnectFour() = default;
ConnectFour::~ConnectFour() = default;

GameState ConnectFour::start() {
    auto* initialState = new std::array<std::array<int, COLS>, ROWS>();
    for (auto& row : *initialState) {
        row.fill(0);
    }
    return GameState(initialState, false);
}

bool ConnectFour::checkWinner(const std::array<std::array<int, COLS>, ROWS>& state) {
    // Check horizontal, vertical, and diagonal connections
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (state[row][col] == 1) {
                // Horizontal
                if (col + 3 < COLS) {
                    bool win = true;
                    for (int i = 0; i < 4; i++) {
                        if (state[row][col + i] != 1) {
                            win = false;
                            break;
                        }
                    }
                    if (win) return true;
                }

                // Vertical
                if (row + 3 < ROWS) {
                    bool win = true;
                    for (int i = 0; i < 4; i++) {
                        if (state[row + i][col] != 1) {
                            win = false;
                            break;
                        }
                    }
                    if (win) return true;
                }

                // Diagonal
                if (row - 3 >= 0 && col + 3 < COLS) {
                    bool win = true;
                    for (int i = 0; i < 4; i++) {
                        if (state[row - i][col + i] != 1) {
                            win = false;
                            break;
                        }
                    }
                    if (win) return true;
                }

                // Other Diagonal
                if (row + 3 < ROWS && col + 3 < COLS) {
                    bool win = true;
                    for (int i = 0; i < 4; i++) {
                        if (state[row + i][col + i] != 1) {
                            win = false;
                            break;
                        }
                    }
                    if (win) return true;
                }
            }
        }
    }
    return false;
}

GameState ConnectFour::move(const GameState& state, int action) {
    if (!isValidAction(state, action)) {
        throw std::invalid_argument("Invalid action");
    }

    auto* currentState = static_cast<std::array<std::array<int, COLS>, ROWS>*>(state.state);
    auto* newState = new std::array<std::array<int, COLS>, ROWS>(*currentState);

    // Get row idx
    int rowIdx = ROWS - 1;
    while (rowIdx >= 0 && (*newState)[rowIdx][action] != 0) {
        rowIdx--;
    }

    (*newState)[rowIdx][action] = 1;

    bool isTerminal = checkWinner(*newState);
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

    return GameState(newState, isTerminal);
}

void ConnectFour::setState(GameState& state, int player) {
    auto* board = static_cast<std::array<std::array<int, COLS>, ROWS>*>(state.state);
    if (player == -1) {
        for (auto& row : *board) {
            for (int& cell : row) {
                cell = -cell;
            }
        }
    }
}

GameState ConnectFour::flipBoard(const GameState& state) {
    auto* currentState = static_cast<std::array<std::array<int, COLS>, ROWS>*>(state.state);
    auto* newState = new std::array<std::array<int, COLS>, ROWS>();

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            (*newState)[i][COLS - 1 - j] = -(*currentState)[i][j];
        }
    }

    return GameState(newState, state.isTerminal);
}

bool ConnectFour::isValidAction(const GameState& state, int action) {
    if (action > 6 || action < 0) return false;
    auto* board = static_cast<std::array<std::array<int, COLS>, ROWS>*>(state.state);
    return (*board)[0][action] == 0;
}

std::vector<int> ConnectFour::getValidActions(const GameState& state) {
    std::vector<int> validActions;
    for (int action = 0; action < COLS; action++) {
        if (isValidAction(state, action)) {
            validActions.push_back(action);
        }
    }
    return validActions;
}

float ConnectFour::getOpponentReward(float reward) {
    return -reward;
}

std::vector<float> ConnectFour::encodeState(const GameState& state) {
    auto* board = static_cast<std::array<std::array<int, COLS>, ROWS>*>(state.state);
    std::vector<float> encoded;
    encoded.reserve(ROWS * COLS);

    for (const auto& row : *board) {
        for (int cell : row) {
            encoded.push_back(static_cast<float>(cell));
        }
    }

    return encoded;
}

int ConnectFour::actionSpaceSize() {
    return COLS;
}

int ConnectFour::stateSpaceSize() {
    return ROWS * COLS;
}