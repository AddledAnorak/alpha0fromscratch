import torch
import torch.nn as nn
import torch.optim as optim

from networks import BasicModel

checkpoint = torch.load('model.pth')
model = BasicModel(state_size=9, action_size=9, hidden_sizes=[128, 128])
model.policy_value.load_state_dict(checkpoint['policy_value'])
model.q_network.load_state_dict(checkpoint['q_network'])

from game_env import Game
from tictactoe import TicTacToe



if __name__ == "__main__":
    model = BasicModel(state_size=9, action_size=9, hidden_sizes=[128, 128])
    game = TicTacToe()
    state = game.start() # from model's view

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

        # # AI move
        # print("AI BOARD:")
        # print(state.state)

        probs, _ = model.predict(state.state.flatten())
        action = torch.argmax(probs).item()

        try:
            state, _ = game.move(state, action)
        except Exception as e:
            print(f"Invalid move by AI: {e}")
            continue

        if state.is_terminal:
            print(game.flip_board(state).state)
            print("Game over!")
            break
