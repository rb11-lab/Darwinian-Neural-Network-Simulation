#pragma once
#include <algorithm>
#include <vector>
#include "NeuralNetwork.hpp"
#include "Vec2.hpp"
#include "Map.hpp"

class Agent
{
    Vec2 pos = {0.0f, 0.0f};
    float turn_degree = 0.0f; // Start facing forward
    float max_turn_speed = 0.6f;
    float max_speed = 9.8f;
    float max_energy = 100.0f;
    float energy = 100.0f;
    float fitness = 0.0f;
    float agent_radius = 5.0f;
    bool is_alive = true;

    std::vector<float> inputs;
    NeuralNetwork brain;

public:
    bool is_alive_value() const { return is_alive; }
    Vec2 pos_value() const { return pos; }
    float radius_value() const { return agent_radius; }
    float fitness_value() const { return fitness; }

    void add_energy(float value) { energy = std::min(max_energy, energy + value); }
    void add_fitness(float value) { fitness += value; }

    void reset(const Vec2& new_pos) {
        pos = new_pos;
        turn_degree = 0.0f; // Reset the agent so it faces forward again
        energy = max_energy;
        fitness = 0.0f;
        is_alive = true;
    }

    const Genome& get_genome() const { return brain.get_genome(); }
    float get_turn_degree() const { return turn_degree; }

    Agent(const InputManager& config, const Genome& g)
        : brain(config, g) {}

    std::vector<float> get_inputs(const Vec2& posF, [[maybe_unused]] const Map& map) {
        float delta_x = posF.x - pos.x;
        float delta_y = posF.y - pos.y;
        float dis_to_food = sqrtf(delta_x * delta_x + delta_y * delta_y);

        float alpha_food = std::atan2(delta_y, delta_x);
        float relative_degree = alpha_food - turn_degree;

        float pi = 3.141592f;

        if (relative_degree > pi) relative_degree -= 2 * pi;
        else if (relative_degree < -pi) relative_degree += 2 * pi;

        float clean_degree = relative_degree / pi;
        float current_energy = energy / max_energy;

        float norm_dist = 1.0f - std::min(1.0f, dis_to_food / 400.0f);

        return {clean_degree, norm_dist, current_energy};
    }

    void update(float dt, const Vec2& closest_food_pos, const Map& map) {
        // Remember the distance before the step
        float prev_dx = closest_food_pos.x - pos.x;
        float prev_dy = closest_food_pos.y - pos.y;
        float prev_dist = std::sqrt(prev_dx * prev_dx + prev_dy * prev_dy);

        inputs = get_inputs(closest_food_pos, map);
        std::vector<float> outputs = brain.feed_forwards(inputs);
        // Missing outputs stay at zero so a short network result cannot be indexed.
        float forward = outputs.size() > 0 ? outputs[0] : 0.0f;
        float turn = outputs.size() > 1 ? outputs[1] : 0.0f;

        turn_degree += turn * max_turn_speed * dt;

        pos.x += std::cos(turn_degree) * forward * max_speed * dt;
        pos.y += std::sin(turn_degree) * forward * max_speed * dt;

        // Distance after the step
        float new_dx = closest_food_pos.x - pos.x;
        float new_dy = closest_food_pos.y - pos.y;
        float new_dist = std::sqrt(new_dx * new_dx + new_dy * new_dy);

        // --- SHAPED REWARD (continuous reinforcement) ---
        // 1. Reward for getting closer to food
        float dist_reward = (prev_dist - new_dist) * 0.1f;

        // 2. Only reward if the agent actually gets closer to the food
        if (dist_reward > 0.0f) {
            fitness += dist_reward * 0.05f;
        }

        // Penalty for steering
        fitness -= std::abs(turn) * 0.001f;
        if (fitness < 0.0f) fitness = 0.0f;

        // The slower the agent moves, the more relative energy it wastes on turning
        energy -= (2.0f + std::abs(turn) * 2.0f + (1.0f - forward) * 1.5f) * dt;

        if (energy <= 0.0f) is_alive = false;

        pos.x = std::clamp(pos.x, 0.0f, map.width);
        pos.y = std::clamp(pos.y, 0.0f, map.height);
    }
};
