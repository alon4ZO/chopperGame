#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <thread>
#include <chrono>

using namespace std;

void threadPrintFunction()
{
    while (1)
    {
        cout << "Test" << endl;
        this_thread::sleep_for(chrono::seconds(3));
    }
}

int main()
{
    std::cout << "[Main] Starting chopper game" << std::endl;

    // Assuming Screen::getScreen() returns a pointer to a valid Screen object
    Screen *screen = Screen::getScreen();
    if (!screen)
    {
        cerr << "[Error] Failed to initialize screen." << endl;
        return -1;
    }

    GameShapes gameShapes(screen->getgameScreenWidth(), screen->getgameScreenHeight());

    // Directly use the threadPrint function
    thread threadPrint(threadPrintFunction);

    // Join threads to ensure they complete before exiting
    screen->play(gameShapes);
    threadPrint.join();

    return 0;
}
