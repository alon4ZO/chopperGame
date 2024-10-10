#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <manager.hpp>
#include <thread>
#include <cassert>

using namespace std;

bool Screen::isInitialized = false;
bool Manager::isInitialized = false;

int main()
{
    std::cout << "[Main] Starting game" << std::endl;

    Screen screen;
    Manager manager;

    std::thread threadManager(&Manager::Start, &manager);
    // ALONB - I don't understand this synthax. it doesn't fit any constructor of thread.

    screen.Render();
    threadManager.join();

    return 0;
}