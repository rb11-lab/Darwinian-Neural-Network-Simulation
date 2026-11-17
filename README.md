# Darwinian Neural Network Simulation

A real-time evolutionary simulation engine built from scratch in C++17, featuring feed-forward neural network "brains", genetic algorithm evolution (crossover, mutations, elitism), and dynamic Raylib visualization.

## Core Features

* **Neural Network Brains (`NeuralNetwork`)**: Multi-layer feed-forward neural architecture utilizing `std::tanh` activation functions for continuous directional and velocity control.
* **Genetic Algorithm (`Genome`)**: Evolutionary cycle featuring uniform crossover (each weight is copied from either parent with equal probability), per-gene Gaussian mutation, and top-tier elitism to preserve high-performing neural weights across generations.
* **Shaped Reward**: Fitness increases when an agent moves closer to the nearest food, with a small penalty for turning.
* **Dynamic Energy & Metabolism**: Custom metabolic system where agent lifespan decays based on baseline hunger, angular velocity, and movement speed.
* **Interactive Visualisation**: Hardware-accelerated rendering powered by Raylib, featuring real-time HUD analytics (Best/Avg Fitness), directional sight vectors, and visual elite agent highlighting.
* **Time Controls**: On-the-fly simulation speed scaling (1x, 10x, 50x) via keyboard input (`SPACE`). The chosen speed is the number of simulation steps run each frame.

## Tech Stack

* C++ (C++17)
* Raylib 5.5 (downloaded by CMake FetchContent)
* CMake

## Build and Run

Requires CMake 3.16 or newer, a C++17 compiler, and git. Raylib 5.5 is fetched and built automatically. On Linux, install the X11 and OpenGL development packages Raylib links against (`libgl1-mesa-dev`, `libx11-dev`, `libxrandr-dev`, `libxi-dev`, `libxinerama-dev`, `libxcursor-dev`).

```bash
cmake -S . -B build
cmake --build build
./build/darwinian_sim
```

With a multi-config generator such as Visual Studio, the executable is `build/Debug/darwinian_sim.exe` or `build/Release/darwinian_sim.exe`.

Press SPACE to cycle simulation speed between 1x, 10x, and 50x. Close the window to quit.

## Project Architecture

```text
Darwinian-Neural-Network-Simulation/
├── CMakeLists.txt      # FetchContent Raylib 5.5, target definitions, and warning flags
├── main.cpp            # Entry point, Raylib window lifecycle, HUD rendering & speed controls
├── World.hpp           # World state, step dispatch, evolutionary selection & generation reset
├── Agent.hpp           # Agent entity, neural input normalization, reward mechanics & energy decay
├── NeuralNetwork.hpp   # Feed-forward neural network implementation & weight matrix processing
├── Genome.hpp          # Genetic weight container, random initialization, mutation & crossover logic
├── Food.hpp            # Food entity structure & bounded map respawn mechanics
├── Vec2.hpp            # Shared 2D vector used by agents, food, and the world
└── Map.hpp             # Environment boundary configuration

```
