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
    static unique_ptr<Screen> instance;
    uint32_t gameScreenWidth;
    uint32_t gameScreenHeight;
    sf::RenderWindow window;

public:
    Screen(uint8_t xRatioPercentage = GAME_BOARD_X_RATIO, uint8_t yRatioPercentage = GAME_BOARD_Y_RATIO);
    Screen(Screen &) = delete;
    void operator=(Screen &) = delete;
    static Screen *getScreen();
    void play(GameShapes &gameShapes);

    uint32_t getgameScreenWidth() { return gameScreenWidth; }
    uint32_t getgameScreenHeight() { return gameScreenHeight; }
};
