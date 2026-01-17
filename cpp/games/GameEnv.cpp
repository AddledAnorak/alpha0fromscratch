#include "GameEnv.h"

// GameState implementation
GameState::GameState(void* state, bool isTerminal) 
    : state(state), isTerminal(isTerminal) {
}

// Game implementation
template<typename T>
Game<T>::Game() {
}

template<typename T>
Game<T>::~Game() {
}