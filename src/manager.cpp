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
    MANAGER_SM_COLLISION,
    MANAGER_SM_SCORE,
} MANAGER_SM_E;

void Manager::Start()
{
    MANAGER_SM_E state = MANAGER_SM_COUNT_DOWN;
    int8_t countDown = 3; // STATIC ASERT not bigger than 3
    uint8_t flickers = 4; // STATIC ASSERT even.
    int8_t meduzCountDown;

    GameShapes *GameShapes = GameShapes::getGameShapes();

    // GameShapes->setActiveGame();
    while (1)
    {
        // std::cout << "R" << std::endl;

        switch (state)
        {
        case MANAGER_SM_COUNT_DOWN:
        {
            std::cout << "[Manager] - counting down - " << static_cast<uint32_t>(countDown) << std::endl;
            GameShapes->setCountDown(countDown);
            // std::cout << "S" << std::endl;

            countDown--;
            if (countDown < 0)
            {
                state = MANAGER_SM_GAME;
                std::cout << "[Manager] - Starting active game " << std::endl;
                GameShapes->setActiveGame();
                continue;
            }
            this_thread::sleep_for(chrono::milliseconds(GAME_BOARD_COUNTDOWN_TIME_INTERVALS_MS));
        }
        break;

        case MANAGER_SM_GAME:
        {

            // std::cout << "[Manager] - New shape" << std::endl;
            if (GameShapes->isCollionWithObsticle())
            {
                state = MANAGER_SM_COLLISION;
                // cout << "[Manager] - collision detected " << GameShapes->getobsticals().size() << endl;
            }
            else
            {
                GameShapes->createNewShark();
                if (meduzCountDown-- <= 0)
                {
                    GameShapes->createNewMeduz();
                    meduzCountDown = getRandomNumber(5, 7);
                }

                GameShapes->cleanUpOldObjects();

                // cout << "Num of obsticles is: " << GameShapes->getobsticals().size() << endl;
                this_thread::sleep_for(chrono::milliseconds(static_cast<uint32_t>(1000 / MANAGER_INITIAL_SHARKS_PER_SEC))); // ALONB - randomize this a bit? or make different sizes for octs..
            }
        }
        break;

        case MANAGER_SM_COLLISION:
        {
            cout << "COLLLLLLLLL" << endl;
            GameShapes->flickerScreen();

            // std::cout << "[Manager] - New shape" << std::endl;
            this_thread::sleep_for(chrono::milliseconds(150));
            flickers--;
            if (flickers <= 0)
            {
                state = MANAGER_SM_GAME;
                GameShapes->setActiveGame();
                flickers = 4;
            }
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

// if (positionOfNewestObsticle.x < screenDimentions.x - spacing)
// {
//     unique_ptr<sf::Shape> obsticle = shapeFactory::createObsticle(screenDimentions.x, screenDimentions.y, wallThickness, spacing);
//     obsticals.push_back(move(obsticle));
//     return;
// }