#pragma once
#include <vector>
#include <random>

// Хранит геном/веса
struct Genome { 
    std::vector<float> weights;

    static Genome create_random(size_t size) {
        Genome genome;
        genome.weights.resize(size);

        // Потокобезопасная локальная инициализация для генерации
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

        for (size_t i = 0; i < size; ++i) {
            genome.weights[i] = dis(gen);
        }

        return genome;
    }

    void mutate(float mutation_rate, float mutation_strength) {
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_real_distribution<float> dis_rate(0.0f, 1.0f);
        std::normal_distribution<float> dis_mutate(0.0f, mutation_strength);

        for (float& weight : weights) {
            if (dis_rate(gen) < mutation_rate) {
                weight += dis_mutate(gen);
            }
        }
    }

    static Genome crossover(const Genome& parentA, const Genome& parentB) {
        Genome child;
        child.weights.resize(parentA.weights.size());
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> coin(0, 1);

        for (size_t i = 0; i < child.weights.size(); ++i) {
            child.weights[i] = coin(gen) ? parentA.weights[i] : parentB.weights[i];
        }
        return child;
    }
}; 

