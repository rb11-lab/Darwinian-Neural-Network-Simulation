#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include "Genome.hpp"

struct InputManager // Stores the network configuration
{
    size_t input_nodes;
    size_t hidden_nodes;
    size_t output_nodes;

    size_t get_required_weights_count() const {
        return (input_nodes * hidden_nodes) + (output_nodes * hidden_nodes);
    }
};

// Uses the configuration and weights to run the network
class NeuralNetwork
{
    InputManager input;
    Genome genome;

public:
    NeuralNetwork(const InputManager& config, const Genome& g)
        : input(config), genome(g) {}

    const Genome& get_genome() const { return genome; }

    std::vector<float> feed_forwards(const std::vector<float>& inputs) {
        // Always hand back one value per output so callers can index safely.
        std::vector<float> outputs(input.output_nodes, 0.0f);

        if (inputs.size() != input.input_nodes) {
            std::cerr << "[ERROR]";
            return outputs;
        }

        if (genome.weights.size() != input.get_required_weights_count()) {
            std::cerr << "[ERROR]";
            return outputs;
        }

        size_t weight_idx = 0;

        std::vector<float> hidden(input.hidden_nodes, 0.0f);

        // Input layer -> hidden layer
        for (size_t i = 0; i < input.hidden_nodes; ++i) {
            float sum = 0.0f;
            for (size_t j = 0; j < input.input_nodes; ++j) {
                sum += inputs[j] * genome.weights[weight_idx++];
            }

            hidden[i] = std::tanh(sum); // Activation after summing inputs
        }

        // Hidden layer -> output layer

        for (size_t k = 0; k < input.output_nodes; ++k) {
            float sum = 0.0f;
            for (size_t i = 0; i < input.hidden_nodes; ++i) {
                sum += hidden[i] * genome.weights[weight_idx++];
            }
            outputs[k] = std::tanh(sum); // Output neuron activation
        }

        return outputs;
    }
};