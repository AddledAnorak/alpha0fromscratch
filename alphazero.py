import numpy as np
import torch
import math
from tqdm import tqdm
from copy import deepcopy

from game_env import Game, GameState
from tictactoe import TicTacToe
from connectfour import ConnectFour
from networks import *
from mcts import MCTS


class AlphaZeroPointZero:
    def __init__(self, game: Game, num_plays: int, k:int, num_simulations: int = 1000, exploration_weight: float = 1.0):
        self.game = game

        self.model = BasicModel(state_size=self.game.state_space_size(), action_size=self.game.action_space_size(),
                                hidden_sizes=[128, 128])

        self.mcts = MCTS(game, self.model, num_simulations=num_simulations, exploration_weight=exploration_weight)
        self.num_plays = num_plays
        self.k = k
        self.num_simulations = num_simulations
        self.exploration_weight = exploration_weight

        # data storage for self-play
        # each entry is a tuple (state, action_probs, reward, winning_player)
        self.data = []


    def self_play_rollout(self):
        state = self.game.start()
        player = 1
        rollout = []

        while True:
            # simulate a rollout
            action_probs = self.mcts.search(state)
            action = np.random.choice(range(self.game.action_space_size()), p=action_probs)
            new_state, reward = self.game.move(state, action)
            rollout.append((state, action_probs, player))

            if new_state.is_terminal:
                # store the data
                reward = reward if player == 1 else self.game.get_opponent_reward(reward)
                for state, action_probs, player in rollout:
                    state = self.game.encode_state(state)
                    self.data.append((state, action_probs, reward, new_state, player)) # player is added for debugging purposes
                break

            new_state = self.game.flip_board(new_state)
            state = new_state

            player = -player


    def train(self):
        # train the model using supervised fine-tuning (SFT)
        states, action_probs, rewards = zip(*[(d[0], d[1], d[2]) for d in self.data])
        states = torch.tensor(states, dtype=torch.float32)
        action_probs = torch.tensor(action_probs, dtype=torch.float32)
        rewards = torch.tensor(rewards, dtype=torch.float32)
        
        dataset = torch.utils.data.TensorDataset(states, action_probs, rewards)
        dataloader = torch.utils.data.DataLoader(dataset, batch_size=64, shuffle=True)

        optimizer = torch.optim.Adam(self.model.parameters(), lr=0.001)

        for epoch in range(50):
            for batch_states, batch_action_probs, batch_rewards in dataloader:
                optimizer.zero_grad()
                probs, values = self.model.predict(batch_states)
                loss = torch.nn.functional.mse_loss(probs, batch_action_probs)
                loss += torch.nn.functional.mse_loss(values, batch_rewards)
                loss.backward()
                optimizer.step()
            print(f'Epoch {epoch + 1}, Loss: {loss.item()}')

    
    def save(self):
        torch.save(self.model.state_dict(), 'model.pth')
        print("Model saved to model.pth")

        # Save the data for future use
        np.save('self_play_data.npy', self.data)
        print("Self-play data saved to self_play_data.npy")


    def learn(self):
        for _ in range(self.k):
            # collect data
            for _ in tqdm(range(self.num_plays // self.k)):
                self.self_play_rollout()
            # train the model using sft
            self.train()

        self.save()


if __name__ == "__main__":
    game = TicTacToe()  # or ConnectFour()
    az = AlphaZeroPointZero(game, num_plays=5000, k=5, num_simulations=50)
    az.learn()
    print("Training complete.")