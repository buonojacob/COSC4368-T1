# COSC4368 Task 1
## Overview
This project considers a 5x5 PD-World grid and aims to demonstrate reinforcement learning concepts through implementation of Q-learning and SARSA. 

## Dependencies and Build
Requires a C++17 compiler and uses [Raylib 5.5](https://www.raylib.com) for Q-table visualization. If Raylib is not found locally, CMake will automatically fetch and build it from source via FetchContent, so no manual installation is required. Can be run with the terminal commands:
```
mkdir build
cd build
cmake ..
cmake --build .
```
and then be run as an executable from the build folder:
```
./pdworld
```

## Structure
The `World` class maintains the state of the PD-World, the `Agent` class handles movement and Q-updates, the `State` class encodes the state space and determines which operators are applicable, the `QTable` class stores and retrieves Q-values, and `main.cpp` runs all three experiments as specified by the assignment.

## State Space
The alternative state space is outlined in the slideshow for the assignment instructions, but follows the structure `(i, j, x, s, t, u)`, where i, j is the agent's position, x is whether or not the agent is holding a block, and s, t, u are the status of the three pickup locations (when not holding a block) or the three dropoff locations (when holding a block).

## Experiments
The first experiment uses a Q-learning based PRANDOM policy for 4000 steps, and then the PGREEDY policy for 4000 steps. The second experiment also starts with a Q-learning based PRANDOM policy for 200 steps and then the PEXPLOIT policy for 7800. Lastly, the final experiment similarly runs the PRANDOM policy for 200 steps, and then the PEXPLOIT policy for 7800, but implements SARSA learning instead.

## Parameters
Fixed values across all experiments include α = 0.3, γ = 0.5, initial Q-values = 0, reward for pickup/dropoff = +13, and movement penalty = -1, all as specified in the instructions.

## Output
The program outputs short summaries after each experiment, stating number of runs, average number of operators to reach a terminal state, the number of operators it took the final run, and the final sum of rewards gained by the agent. Also, at every policy change during an experiment, the program displays the QTable at that moment, as well as additional ones throughout.