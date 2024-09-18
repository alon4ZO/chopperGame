#pragma once
#include <SFML/Graphics.hpp>
#include <definitions.h>
#include <memory>
#include <gameShapes.hpp>

using namespace std;

/* singleton class to hold the main Screen object */
class Screen
{
private:
    // uint32_t screenDimentions.x;
    // uint32_t screenDimentions.y;
    sf::RenderWindow window;

public:
    Screen(float ratio = GAME_BOARD_RATIO);
    void Render();
};
