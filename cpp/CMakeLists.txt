cmake_minimum_required(VERSION 3.16)
project(Alpha0FromScratch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR})

# Create a library for game environments
add_library(games 
    games/GameEnv.h
    games/GameEnv.cpp
    games/TicTacToe/TicTacToe.h
    games/TicTacToe/TicTacToe.cpp
    games/ConnectFour/ConnectFour.h
    games/ConnectFour/ConnectFour.cpp
)

# Create a library for algorithms
add_library(algorithms
    algorithms/mcts.h
    algorithms/mcts.cpp
)

# Main executable
add_executable(alpha0 main.cpp)

# Link libraries
target_link_libraries(alpha0 algorithms games)

# Set compiler flags for debugging and optimization
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(alpha0 PRIVATE -g -O0 -Wall -Wextra)
else()
    target_compile_options(alpha0 PRIVATE -O3 -DNDEBUG)
endif()
