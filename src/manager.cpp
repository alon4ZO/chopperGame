#include <SFML/Graphics.hpp>
#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <manager.hpp>
#include <thread>
#include <chrono>

using namespace std;

typedef enum ManagerSm
{
    MANAGER_SM_COUNT_DOWN = 0,
    MANAGER_SM_GAME,
    MANAGER_SM_SCORE,
} MANAGER_SM_E;

void Manager::Start()
{
    MANAGER_SM_E state = MANAGER_SM_COUNT_DOWN;
    uint8_t countDown = 3;

    GameShapes *GameShapes = GameShapes::getGameShapes();

    while (1)
    {
        std::cout << "R" << std::endl;

        switch (state)
        {
        case MANAGER_SM_COUNT_DOWN:
        {
            std::cout << "[Manager] - counting down - " << static_cast<uint32_t>(countDown) << std::endl;
            GameShapes->clearAll();
            GameShapes->setCountDown(countDown);
            this_thread::sleep_for(chrono::seconds(1));
            // std::cout << "S" << std::endl;

            countDown--;
            if (countDown == 0)
            {
                state = MANAGER_SM_GAME;
                GameShapes->clearAll();
                GameShapes->setActiveGame();
            }
        }
        break;

        case MANAGER_SM_GAME:
        {
            std::cout << "[Manager] - playing - " << static_cast<uint32_t>(countDown) << std::endl;
            this_thread::sleep_for(chrono::seconds(3));
        }
        break;
        }
    }
}
