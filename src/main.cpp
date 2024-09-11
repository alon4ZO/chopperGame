#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <manager.hpp>
#include <thread>
#include <chrono>

using namespace std;

int main()
{
    std::cout << "[Main] Starting chopper game" << std::endl;

    // Assuming Screen::getScreen() returns a pointer to a valid Screen object
    Screen *screen = Screen::getScreen();
    Manager manager;

    // GameShapes gameShapes(screen->getgameScreenWidth(), screen->getgameScreenHeight());

    // Directly use the threadPrint function
    thread threadManager([&manager]()
                         { manager.Start(); });

    // Join threads to ensure they complete before exiting
    screen->Render();
    threadManager.join();

    return 0;
}

// ALONB screen probably does not need to be singleton, and shapes does need to be so it can be accessed from different modules. screen and manager can have a 1 time init protection.