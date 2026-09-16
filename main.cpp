#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "raylib.h"
#include "World.hpp"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    // Simulation speed; default is 1x
    int simulation_speed = 1;

    InitWindow(screenWidth, screenHeight, "Darwinian Neural Network Simulation");
    SetTargetFPS(60);

    InputManager config;
    config.input_nodes = 3;
    config.hidden_nodes = 4;
    config.output_nodes = 2;

    World world(config);
    world.init(50, 100, static_cast<float>(screenWidth), static_cast<float>(screenHeight));

    int current_gen = 1;
    int step_counter = 0;
    const int steps_per_generation = 600;
    const float dt = 0.016f;

    while (!WindowShouldClose()) {
        world.step(dt);
        step_counter++;

        if (step_counter >= steps_per_generation) {
            world.next_generation();
            step_counter = 0;
            current_gen++;
        }

        // Toggle simulation speed with SPACE (1x -> 10x -> 50x -> 1x)
        if (IsKeyPressed(KEY_SPACE)) {
            if (simulation_speed == 1) simulation_speed = 10;
            else if (simulation_speed == 10) simulation_speed = 50;
            else simulation_speed = 1;
        }

        // Run the simulation logic several times per frame
        for (int sub_step = 0; sub_step < simulation_speed; ++sub_step) {
            world.step(dt);
            step_counter++;

            if (step_counter >= steps_per_generation) {
                world.next_generation();
                step_counter = 0;
                current_gen++;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // 1. Draw food
        for (const auto& food : world.get_foods()) {
            if (!food.is_eaten) {
                DrawCircleV({ food.pos.x, food.pos.y }, food.col_r, GREEN);
            }
        }

        // 2. Draw agents (elite = gold, regular = red)
        const auto& agents = world.get_agents();
        size_t elite_limit = world.get_elite_count();

        for (size_t i = 0; i < agents.size(); ++i) {
            const auto& agent = agents[i];
            if (agent.is_alive_value()) {
                Vector2 pos = { agent.pos_value().x, agent.pos_value().y };

                // Elite agents are highlighted in gold
                Color agent_color = (i < elite_limit) ? GOLD : RED;

                DrawCircleV(pos, agent.radius_value(), agent_color);

                // Direction the agent is facing
                float dir_x = pos.x + std::cos(agent.get_turn_degree()) * 12.0f;
                float dir_y = pos.y + std::sin(agent.get_turn_degree()) * 12.0f;
                DrawLineV(pos, { dir_x, dir_y }, BLACK);
            }
        }

        // 3. Expanded HUD with stats
        DrawRectangle(10, 10, 260, 115, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(10, 10, 260, 115, BLUE);

        std::stringstream best_str, avg_str;
        best_str << std::fixed << std::setprecision(1) << world.get_best_fitness();
        avg_str << std::fixed << std::setprecision(1) << world.get_avg_fitness();

        DrawText(("Gen: " + std::to_string(current_gen)).c_str(), 20, 20, 20, DARKBLUE);
        DrawText(("Step: " + std::to_string(step_counter) + " / " + std::to_string(steps_per_generation)).c_str(), 20, 42, 16, DARKBLUE);
        DrawText(("Best Fitness: " + best_str.str()).c_str(), 20, 64, 16, MAROON);
        DrawText(("Avg Fitness:  " + avg_str.str()).c_str(), 20, 84, 16, DARKBLUE);

        // Show current speed in the HUD
        DrawText(("Speed: " + std::to_string(simulation_speed) + "x (Press SPACE)").c_str(), 20, 130, 16, PURPLE);

        // Color legend
        DrawCircle(20, 112, 4, GOLD);
        DrawText("Elite", 28, 106, 12, DARKGRAY);
        DrawCircle(70, 112, 4, RED);
        DrawText("Mutated", 78, 106, 12, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}