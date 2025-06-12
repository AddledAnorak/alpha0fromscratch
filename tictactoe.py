import numpy as np

from game_env import Game, GameState

class TicTacToe(Game):
    def __init__(self):
        pass

    def start(self):
        # Initialize the game state
        initial_state = np.zeros((3, 3), dtype=int)
        return GameState(initial_state, is_terminal=False)

    def _check_winner(self, state):
        for i in range(3):
            if np.all(state[i, :] == 1) or np.all(state[:, i] == 1):
                return True
        if np.all(np.diag(state) == 1) or np.all(np.diag(np.fliplr(state)) == 1):
            return True
        return False

    def check_winner(self, state):
        if not state.is_terminal:
            return False
        state = state.state
        return self._check_winner(state)

    def move(self, state, action):
        if not self.is_valid_action(state, action):
            raise ValueError("Invalid action.")
        
        state = state.state
        row, col = divmod(action, 3)
        new_state = state.copy()
        new_state[row][col] = 1
        is_terminal = self._check_winner(new_state) or np.all(new_state != 0)
        reward = 1 if self._check_winner(new_state) else 0
        return GameState(new_state, is_terminal), reward

    def flip_board(self, state):
        return GameState(-state.state[::-1, ::-1], state.is_terminal)

    def is_valid_action(self, state, action):
        if state.is_terminal:
            return False
        row, col = divmod(action, 3)
        return state.state[row][col] == 0

    def get_valid_actions(self, state):
        return [action for action in range(9) if self.is_valid_action(state, action)]

    def get_opponent_reward(self, reward):
        return -reward
    
    def encode_state(self, state):
        return state.state.flatten()
    
    def action_space_size(self):
        return 9
    
    def state_space_size(self): 
        return 9