#pragma once
#include <SFML/Graphics.hpp>
#include <definitions.h>
#include <memory>
#include <gameShapes.hpp>

using namespace std;

/* singleton class to hold the main Screen object */
class dimensions
{ // ALONB -make this class a bit smarter, getters, setters, etc, check init, etc..
public:
    static sf::Vector2u screenDimentions;
    static sf::Vector2u activeGameDimentions;
    static uint32_t activeGameYOffset;
};
