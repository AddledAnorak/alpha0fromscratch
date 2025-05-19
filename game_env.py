from abc import ABC

class GameState:
    def __init__(self, state=None, is_terminal=False,):
        self.state = state
        self.is_terminal = is_terminal

class Game(ABC):
    def __init__(self):
        pass

    def start(self):
        pass

    def move(self, state, action):
        pass

    def set_state(self, state, player):
        pass

    def flip_board(self, state):
        pass

    def is_valid_action(self, state, action):
        pass

    def get_valid_actions(self, state):
        pass

    def get_opponent_reward(self, reward):
        pass

    def encode_state(self, state):
        pass