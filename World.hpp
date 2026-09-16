#pragma once
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <numeric>
#include "Food.hpp"
#include "Agent.hpp"

class World
{
    InputManager config;
    std::vector<Food> foods;
    std::vector<Agent> agents;
    Map map;

    std::mt19937 rng{ std::random_device{}() };

    // Stats for the UI
    float best_fitness_history = 0.0f;
    float avg_fitness_history = 0.0f;
    size_t elite_count_current = 0;

public:
    World(const InputManager& cfg)
        : config(cfg) {}

    const std::vector<Food>& get_foods() const { return foods; }
    const std::vector<Agent>& get_agents() const { return agents; }

    float get_best_fitness() const { return best_fitness_history; }
    float get_avg_fitness() const { return avg_fitness_history; }
    size_t get_elite_count() const { return elite_count_current; }

    void init(size_t pop_size, size_t food_amount, float map_w, float map_h) {
        map.width = map_w;
        map.height = map_h;

        foods.clear();
        foods.resize(food_amount);
        for (auto& food : foods) {
            food.is_eaten = true;
            food.respawn(map);
        }

        agents.clear();
        size_t weights_count = config.get_required_weights_count();

        for (size_t i = 0; i < pop_size; ++i) {
            Genome random_genome = Genome::create_random(weights_count);
            Agent new_agent(config, random_genome);
            new_agent.reset(get_random_position());
            agents.push_back(new_agent);
        }

        // There is no elite in the first generation — all 50 agents are random and equal
        elite_count_current = 0;
    }

    void step(float dt) {
        for (auto& agent : agents) {
            if (agent.is_alive_value()) {
                float min_dist = 999999.0f;
                Food* closest_food = nullptr;

                for (auto& food : foods) {
                    if (!food.is_eaten) {
                        float delta_x = agent.pos_value().x - food.pos.x;
                        float delta_y = agent.pos_value().y - food.pos.y;
                        float dist = std::sqrt(delta_x * delta_x + delta_y * delta_y);

                        if (dist < min_dist) {
                            min_dist = dist;
                            closest_food = &food;
                        }
                    }
                }

                if (closest_food != nullptr) {
                    agent.update(dt, closest_food->pos, map);

                    if (min_dist < agent.radius_value() + closest_food->col_r) {
                        agent.add_energy(60.0f);
                        agent.add_fitness(20.0f);
                        closest_food->is_eaten = true;
                        closest_food->respawn(map);
                    }
                }
            }
        }
    }

    void next_generation() {
        // 1. Sort by performance
        std::sort(agents.begin(), agents.end(), [](const Agent& a, const Agent& b) {
            return a.fitness_value() > b.fitness_value();
        });

        // 2. Collect stats before resetting
        best_fitness_history = agents.front().fitness_value();
        float total_fit = 0.0f;
        for (const auto& a : agents) total_fit += a.fitness_value();
        avg_fitness_history = total_fit / agents.size();

        std::vector<Agent> next_agents;
        next_agents.reserve(agents.size());

        // 3. Keep the elite (top 20%) — they will be at the front of next_agents
        elite_count_current = std::max<size_t>(1, agents.size() / 5);
        for (size_t i = 0; i < elite_count_current; ++i) {
            Agent elite = agents[i];
            elite.reset(get_random_position());
            next_agents.push_back(elite);
        }

        // 4. Fill the population with offspring
        size_t parent_pool_size = std::max<size_t>(2, agents.size() / 2);
        std::uniform_int_distribution<size_t> dist(0, parent_pool_size - 1);

        float mutation_rate = 0.05f;
        float mutation_strength = 0.1f;

        while (next_agents.size() < agents.size()) {
            size_t parent_a_idx = dist(rng);
            size_t parent_b_idx = dist(rng);

            Genome child_genome = Genome::crossover(
                agents[parent_a_idx].get_genome(),
                agents[parent_b_idx].get_genome()
            );

            child_genome.mutate(mutation_rate, mutation_strength);

            Agent child(config, child_genome);
            child.reset(get_random_position());
            next_agents.push_back(child);
        }

        agents = std::move(next_agents);
    }

private:
    Vec2 get_random_position() {
        std::uniform_real_distribution<float> dis_x(0.0f, map.width);
        std::uniform_real_distribution<float> dis_y(0.0f, map.height);
        return { dis_x(rng), dis_y(rng) };
    }
};