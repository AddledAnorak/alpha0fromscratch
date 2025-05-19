import numpy as np
import torch
import math
from copy import deepcopy

from game_env import Game, GameState
from tictactoe import TicTacToe
from networks import *
from mcts import MCTS


class AlphaZeroPointZero:
    def __init__(self, game, num_plays, num_simulations=1000, exploration_weight=1.0):
        self.game = game

        self.model = BasicModel(state_size=9, action_size=9)

        self.mcts = MCTS(game, self.model)
        self.num_plays = num_plays
        self.num_simulations = num_simulations
        self.exploration_weight = exploration_weight

        self.data = []


    def self_play_rollout(self):
        state = self.game.start()
        player = 1
        rollout = []

        while True:
            # simulate a rollout
            action_probs = self.mcts.search(state)
            action = np.random.choice(range(9), p=action_probs)
            new_state, reward = self.game.move(state, action)
            new_state = self.game.flip_board(new_state)
            rollout.append((state, action_probs, player))

            state = new_state

            if state.is_terminal:
                # store the data
                reward = reward if player == 1 else self.game.get_opponent_reward(reward)
                for state, action_probs, player in rollout:
                    state = self.game.encode_state(state)
                    self.data.append((state, action_probs, reward))
                break

            player = -player


    def train(self):
        pass


    def learn(self):
        # collect data
        for _ in range(self.num_plays):
            self.self_play_rollout()

        # train the model
        self.train()
