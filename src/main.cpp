#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <manager.hpp>
#include <thread>
#include <chrono>
#include <future>

using namespace std;

int main()
{
    std::cout << "[Main] Starting chopper game" << std::endl;

    Screen screen;
    Manager manager;

    // GameShapes gameShapes(screen->getscreenDimentions.x(), screen->getscreenDimentions.y());

    std::promise<bool> promiseObj;
    std::future<bool> futureObj = promiseObj.get_future();
    // Directly use the threadPrint function
    std::thread threadManager(&Manager::Start, &manager, move(futureObj)); // ALONB - I don't understand this synthax. it doesn't fit any constructor of thread.

    // Join threads to ensure they complete before exiting
    screen.Render(move(promiseObj));
    threadManager.join();

    return 0;
}

// ALONB screen probably does not need to be singleton, and shapes does need to be so it can be accessed from different modules. screen and manager can have a 1 time init protection.