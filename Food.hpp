#pragma once
#include <vector>
#include <random>
#include "Map.hpp"

struct Vec2
{
    float x;
    float y;
};

struct Food
{
    Vec2 pos = {0.0f, 0.0f};
    float col_r = 4.0f;
    bool is_eaten = false;

    // Respawns this food at a random position after it has been eaten
    void respawn(Map& map) {
        if (is_eaten) {
            std::random_device rd;
            std::mt19937 gen(rd());

            std::uniform_real_distribution<float> dis_x(0.0f, map.width - col_r);
            std::uniform_real_distribution<float> dis_y(0.0f, map.height - col_r);

            pos = {dis_x(gen), dis_y(gen)};

            is_eaten = false;
        }
    }
};