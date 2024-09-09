#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>

int main()
{

    std::cout << "[Main] Starting chopper game" << std::endl;

    Screen *screen = Screen::getScreen();

    GameShapes gameShapes(screen->getgameScreenWidth(), screen->getgameScreenHeight());
    // GameShapes->addBasicShapes();
    screen->play(gameShapes);

    return 0;
}
