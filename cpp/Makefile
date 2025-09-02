CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -DNDEBUG
DEBUG_FLAGS = -std=c++17 -Wall -Wextra -g -O0

SRCDIR = .
OBJDIR = build
SOURCES = main.cpp algorithms/mcts.cpp games/GameEnv.cpp games/TicTacToe/TicTacToe.cpp games/ConnectFour/ConnectFour.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJDIR)/%.o)
TARGET = alpha0

BOTBATTLE_LIBSOURCES = algorithms/mcts.cpp games/GameEnv.cpp games/TicTacToe/TicTacToe.cpp games/ConnectFour/ConnectFour.cpp
BOTBATTLE_LIBOBJECTS = $(BOTBATTLE_LIBSOURCES:%.cpp=$(OBJDIR)/%.o)
BATTLE_TARGET = botBattle

.PHONY: all clean debug battle

all: $(TARGET)

debug: CXXFLAGS = $(DEBUG_FLAGS)
debug: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

battle: $(BATTLE_TARGET)
	./$(BATTLE_TARGET)

$(BATTLE_TARGET): $(OBJDIR)/botBattle.o $(BOTBATTLE_LIBOBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/botBattle.o: botBattle.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJDIR) $(TARGET) $(BATTLE_TARGET)

# Dependencies (simplified - in a real project you'd use dependency generation)
$(OBJDIR)/main.o: main.cpp algorithms/mcts.h games/ConnectFour/ConnectFour.h games/TicTacToe/TicTacToe.h
$(OBJDIR)/algorithms/mcts.o: algorithms/mcts.cpp algorithms/mcts.h games/GameEnv.h
$(OBJDIR)/games/GameEnv.o: games/GameEnv.cpp games/GameEnv.h
$(OBJDIR)/games/TicTacToe/TicTacToe.o: games/TicTacToe/TicTacToe.cpp games/TicTacToe/TicTacToe.h games/GameEnv.h
$(OBJDIR)/games/ConnectFour/ConnectFour.o: games/ConnectFour/ConnectFour.cpp games/ConnectFour/ConnectFour.h games/GameEnv.h
