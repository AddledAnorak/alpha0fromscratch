#include "GameEnv.h"

// GameState implementation
GameState::GameState(void* state, bool isTerminal) 
    : state(state), isTerminal(isTerminal) {
}

// Game implementation
Game::Game() {
}

Game::~Game() {
}