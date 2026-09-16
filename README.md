# Darwinian Neural Network Simulation

A real-time evolutionary simulation engine built from scratch in C++17, featuring feed-forward neural network "brains", genetic algorithm evolution (crossover, mutations, elitism), and dynamic Raylib visualization.

## Core Features

* **Neural Network Brains (`NeuralNetwork`)**: Multi-layer feed-forward neural architecture utilizing `std::tanh` activation functions for continuous directional and velocity control.
* **Genetic Algorithm (`Genome`)**: Evolutionary cycle featuring single-point crossover, Gaussian mutation rates, and strict top-tier elitism to preserve high-performing neural weights across generations.
* **Shaped Reward Acceleration**: Dense fitness feedback based on relative food proximity ($\Delta d$) and directional alignment, eliminating sparse reward bottlenecks.
* **Dynamic Energy & Metabolism**: Custom metabolic system where agent lifespan decays based on baseline hunger, angular velocity, and movement speed.
* **Interactive Visualisation**: Hardware-accelerated rendering powered by Raylib, featuring real-time HUD analytics (Best/Avg Fitness), directional sight vectors, and visual elite agent highlighting.
* **Time Controls**: On-the-fly simulation speed scaling (1x, 10x, 50x) via keyboard input (`SPACE`) for rapid multi-generational evolutionary testing.

## Tech Stack

* C++ (C++17)
* Raylib
* CMake

## Project Architecture

```text
NeuroEvolution/
├── CMakeLists.txt      # FetchContent Raylib setup, target definitions & build flags
├── main.cpp            # Entry point, Raylib window lifecycle, HUD rendering & speed controls
├── World.hpp           # World state, step dispatch, evolutionary selection & generation reset
├── Agent.hpp           # Agent entity, neural input normalization, reward mechanics & energy decay
├── NeuralNetwork.hpp   # Feed-forward neural network implementation & weight matrix processing
├── Genome.hpp          # Genetic weight container, random initialization, mutation & crossover logic
├── Food.hpp            # Food entity structure & bounded map respawn mechanics
└── Map.hpp             # Environment boundary configuration

```