#include <SFML/Graphics.hpp>
#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <manager.hpp>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::literals;

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

    GameShapes->setActiveGame();
    while (1)
    {
        // std::cout << "R" << std::endl;

        switch (state)
        {
        case MANAGER_SM_COUNT_DOWN:
        {
            std::cout << "[Manager] - counting down - " << static_cast<uint32_t>(countDown) << std::endl;
            GameShapes->clearAll();
            GameShapes->setCountDown(countDown);
            this_thread::sleep_for(chrono::milliseconds(500));
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
            std::cout << "[Manager] - New shape" << std::endl;

            GameShapes->createNewObstible();
            GameShapes->cleanUpOldObsticles();

            this_thread::sleep_for(chrono::milliseconds(static_cast<uint32_t>(1000 / MANAGER_INITIAL_SHAPES_PER_SEC))); // ALONB - randomize this a bit?
        }
        break;
        }
    }
}

// Move all obsitcles to the left according to time passed and speed.
// for (auto &i : obsticals) This needs to be from Render
// {
//     i->move(-speed * dt, 0.0f);
// }

// // if last obsticle is advanced enough, introduce a new obsitcle.
// auto &newestObsticle = obsticals.back();
// sf::Vector2f positionOfNewestObsticle = newestObsticle->getPosition();

// if (positionOfNewestObsticle.x < gameScreenWidth - spacing)
// {
//     unique_ptr<sf::Shape> obsticle = shapeFactory::createObsticle(gameScreenWidth, gameScreenHeight, wallThickness, spacing);
//     obsticals.push_back(move(obsticle));
//     return;
// }