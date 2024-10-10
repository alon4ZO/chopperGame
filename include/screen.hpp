#pragma once
#include <SFML/Graphics.hpp>
#include <definitions.h>

using namespace std;

class Screen
{
private:
    sf::RenderWindow window;
    static bool isInitialized;

public:
    Screen(float ratio = GAME_SCREEN_X_FROM_HOST_SCREEN_RATIO);
    void Render();
};
