#pragma once
#include <SFML/Graphics.hpp>
#include <definitions.h>
#include <memory>
#include <list>

class Manager // ALONB Add static variable to make sure 1 initialization?
{
private:
    uint32_t score;
    uint32_t speed;                 // how many pixels per second
    uint32_t maxObsticlesPerSecond; // how many obsiticles will be generated per second

public:
    Manager(uint32_t score = MANAGER_INITIAL_SCORE,
            uint32_t speed = MANAGER_INITIAL_OBSTICLE_SPEED_PIX_PER_SEC,
            uint32_t maxObsticlesPerSecond = MANAGER_INITIAL_SHAPES_PER_SEC) {};
    void Start();
};