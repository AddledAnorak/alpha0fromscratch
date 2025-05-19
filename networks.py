import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim


# take s, output pi(s) and V(s)
class BasicPolicyValueNetwork(nn.Module):
    def __init__(self, input_size, hidden_sizes, output_size):
        super().__init__()
        
        self.fc1 = nn.Linear(input_size, hidden_sizes[0])
        self.hidden_layers = []
        
        for i in range(len(hidden_sizes)-1):
            self.hidden_layers.append(nn.Linear(hidden_sizes[i], hidden_sizes[i + 1]))
            self.hidden_layers.append(nn.ReLU())
        
        self.policy_layer = nn.Linear(hidden_sizes[-1], output_size)
        self.value_layer = nn.Linear(hidden_sizes[-1], 1)

    def forward(self, x):
        x = F.relu(self.fc1(x))
        
        for layer in self.hidden_layers:
            x = layer(x)
        
        policy = self.policy_layer(x)
        policy = F.softmax(policy, dim=-1)
        value = self.value_layer(x)
        value = torch.tanh(value)
        
        return policy, value


class BasicQNetwork(nn.Module):
    def __init__(self, input_size, hidden_sizes):
        super().__init__()
        
        self.fc1 = nn.Linear(input_size, hidden_sizes[0])
        self.hidden_layers = []
        
        for i in range(len(hidden_sizes)-1):
            self.hidden_layers.append(nn.Linear(hidden_sizes[i], hidden_sizes[i + 1]))
            self.hidden_layers.append(nn.ReLU())
        
        self.value_layer = nn.Linear(hidden_sizes[-1], 1)

    def forward(self, x):
        x = F.relu(self.fc1(x))
        
        for layer in self.hidden_layers:
            x = layer(x)
        
        value = self.value_layer(x)
        value = torch.tanh(value)
        
        return value


class BasicModel:
    def __init__(self, state_size, action_size, hidden_sizes):
        self.policy_value = BasicPolicyValueNetwork(state_size, hidden_sizes, action_size)
        self.q_network = BasicQNetwork(state_size + action_size, hidden_sizes)

        self.networks = [self.policy_value, self.q_network]

    def predict(self, state):
        state = torch.tensor(state, dtype=torch.float32)
        policy, value = self.policy_value(state)
        return policy, value

    def q(self, state, action):
        state = torch.tensor(state, dtype=torch.float32)
        action = torch.tensor(action, dtype=torch.float32)
        x = torch.cat((state, action), dim=-1)
        q_value = self.q_network(x)
        return q_value


class RandomModel:
    def __init__(self, state_size, action_size):
        self.state_size = state_size
        self.action_size = action_size

    def predict(self, state):
        policy = torch.ones(self.action_size) / self.action_size
        value = torch.tensor(0)
        return policy, value
    
    def q(self, state, action):
        return torch.tensor(0)