#pragma once
#include <random>
#include "Map.hpp"
#include "Vec2.hpp"

struct Food
{
    Vec2 pos = {0.0f, 0.0f};
    float col_r = 4.0f;
    bool is_eaten = false;

    // Respawns this food at a random position after it has been eaten
    void respawn(Map& map, std::mt19937& rng) {
        if (is_eaten) {
            std::uniform_real_distribution<float> dis_x(0.0f, map.width - col_r);
            std::uniform_real_distribution<float> dis_y(0.0f, map.height - col_r);

            pos = {dis_x(rng), dis_y(rng)};

            is_eaten = false;
        }
    }
};
