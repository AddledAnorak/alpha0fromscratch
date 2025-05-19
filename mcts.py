import numpy as np
import torch
import math
from copy import deepcopy

from game_env import Game, GameState
from tictactoe import TicTacToe
from networks import *


class MCTS_Node:
    def __init__(
            self, 
            game: Game, 
            state: GameState, 
            action_taken=None,
            player=1, 
            reward=0, 
            parent=None, 
            prob_prior=1, 
            exploration_weight=1.0,
            node_id=0
        ):
        self.node_id = node_id
        self.parent = parent
        self.action_taken = action_taken

        self.game = game
        self.player = player
        self.state = state
        self.reward = reward
                
        self.children: list[MCTS_Node] = []
        self.value_sum = 0
        self.visits = 0
        self.prob_prior = prob_prior
        self.exploration_weight = exploration_weight
    
    def __str__(self):
        return f"{'AI' if self.player==1 else 'H'}(id={self.node_id}, action_taken={self.action_taken}, value_sum={self.value_sum}, visits={self.visits}, reward={self.reward})"
    
    def rec_print(self, depth=0):
        print("  " * depth + f"{self}")
        for child in self.children:
            child.rec_print(depth + 1)

    def is_fully_expanded(self):
        return len(self.children) > 0 or (self.state.is_terminal and self.visits > 0)
    
    def get_ucb(self, child):
        if child.visits == 0:
            q_value = 0
        else:
            q_value = -child.value_sum / child.visits
        return q_value + self.exploration_weight * child.prob_prior * (math.sqrt(self.visits) / (1 + child.visits))
        
    def best_child(self):
        return max(self.children, key=lambda c: self.get_ucb(c))
    
    def expand(self, policy):
        valid_actions = self.game.get_valid_actions(self.state)
        for action in valid_actions:
            new_state, reward = self.game.move(self.state, action)
            new_state = self.game.flip_board(new_state)
            new_player = -self.player
            new_reward = self.game.get_opponent_reward(reward)
            child_node = MCTS_Node(
                self.game, 
                new_state, action,
                new_player, new_reward, 
                parent=self,
                prob_prior=policy[action],
                exploration_weight=self.exploration_weight,
                node_id=self.node_id + len(self.children) + 1
            )
            self.children.append(child_node)

    def backpropagate(self, value):
        self.visits += 1
        self.value_sum += value

        if self.parent is not None:
            self.parent.backpropagate(self.game.get_opponent_reward(value))


class MCTS:
    def __init__(self, game: Game, model, num_simulations=1000, exploration_weight=1.0):
        self.game = game
        self.num_simulations = num_simulations
        self.model = model
        self.exploration_weight = exploration_weight


    def search(self, state):
        root = MCTS_Node(self.game, state, exploration_weight=self.exploration_weight)

        for i in range(self.num_simulations):
            parent = root

            while parent.is_fully_expanded():
                if parent.state.is_terminal:
                    break
                parent = parent.best_child()


            if not parent.state.is_terminal:
                # get the policy outputs, get the value of this state
                # expand to new nodes according to the policy
                encoded_state = torch.tensor(self.game.encode_state(parent.state))
                policy, value = self.model.predict(encoded_state)
                policy = policy.detach().numpy()
                validity = [self.game.is_valid_action(parent.state, action) for action in range(9)]
                policy = policy * validity
                policy /= np.sum(policy)
                value = value.detach().item()

                parent.expand(policy)
            else:
                value = parent.reward

            parent.backpropagate(value)
        
        probs = np.zeros(9)
        for child in root.children:
            probs[child.action_taken] = child.visits
        probs /= np.sum(probs)

        return probs


if __name__ == "__main__":
    # model = BasicModel(state_size=9, action_size=9, hidden_sizes=[128, 128])
    model = RandomModel(state_size=9, action_size=9)
    game = TicTacToe()
    state = game.start() # from model's view
    mcts = MCTS(game, model, num_simulations=1000, exploration_weight=1.0)

    while True:
        # human move first
        human_state = game.flip_board(state)
        print(human_state.state)
        while True:
            action = int(input("Enter your move (0-8): "))
            if not game.is_valid_action(human_state, action):
                print("Invalid move. Try again.")
                continue
            break
        
        new_state, _ = game.move(human_state, action)
        state = game.flip_board(new_state) # for the AI

        if state.is_terminal:
            print(game.flip_board(state).state)
            print("Game over!")
            break

        # AI move
        print("AI BOARD:")
        print(state.state)

        action, prob = mcts.search(state)
        state, _ = game.move(state, action)

        if state.is_terminal:
            print(game.flip_board(state).state)
            print("Game over!")
            break
