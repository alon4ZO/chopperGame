#pragma once
#include <SFML/Graphics.hpp>
#include <definitions.h>
#include <memory>

using namespace std;

class GameShapes
{
private:
    vector<sf::Shape *> chopper;
    vector<sf::Shape *> walls;
    vector<sf::Shape *> obsticals;

public:
    GameShapes(uint32_t x, uint32_t y) {}
};
