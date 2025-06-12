import numpy as np

from game_env import Game, GameState

class ConnectFour(Game):
    def start(self):
        initial_state = np.zeros((6, 7), dtype=int)
        return GameState(initial_state, is_terminal=False)
    
    def is_valid_action(self, state, action):
        if action > 6 or action < 0:
            return False    
        return state.state[0, action] == 0
    

    def _check_winner(self, state):
        # Check horizontal, vertical, and diagonal connections
        for row in range(6):
            for col in range(7):
                if state[row, col] == 1:
                    # Horizontal
                    if col + 3 < 7 and np.all(state[row, col:col+4] == 1):
                        return True
                    # Vertical
                    if row + 3 < 6 and np.all(state[row:row+4, col] == 1):
                        return True
                    # Diagonal /
                    if row - 3 >= 0 and col + 3 < 7 and np.all([state[row-i, col+i] == 1 for i in range(4)]):
                        return True
                    # Diagonal \
                    if row + 3 < 6 and col + 3 < 7 and np.all([state[row+i, col+i] == 1 for i in range(4)]):
                        return True
        return False
    

    def check_winner(self, state: GameState):
        if not state.is_terminal:
            return False
        state = state.state
        return self._check_winner(state)


    def move(self, state: GameState, action):
        if not self.is_valid_action(state, action):
            raise ValueError("Invalid action.")
        
        state = state.state
        
        # get row idx
        row_idx = 5
        while row_idx >= 0 and state[row_idx, action] != 0:
            row_idx -= 1
        
        new_state = state.copy()
        new_state[row_idx, action] = 1

        is_terminal = self._check_winner(new_state) or np.all(new_state != 0)
        reward = 1 if self._check_winner(new_state) else 0
        return GameState(new_state, is_terminal), reward

    def flip_board(self, state: GameState):
        return GameState(-state.state[:, ::-1], state.is_terminal)
    
    def get_valid_actions(self, state):
        return [action for action in range(7) if self.is_valid_action(state, action)]
    
    def get_opponent_reward(self, reward):
        return -reward
    
    def encode_state(self, state: GameState):
        return state.state.flatten()
    
    def action_space_size(self):
        return 7