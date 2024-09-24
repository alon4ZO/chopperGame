#include <SFML/Graphics.hpp>
#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <manager.hpp>
#include <thread>
#include <chrono>
#include <future>

using namespace std;
using namespace std::literals;

typedef enum ManagerSm
{
    MANAGER_SM_COUNT_DOWN = 0,
    MANAGER_SM_GAME,
    MANAGER_SM_COLLISION,
    MANAGER_SM_GAME_OVER,
    MANAGER_SM_ERROR
} MANAGER_SM_E;

void Manager::Start(std::future<bool> &&futureObj)
{
    MANAGER_SM_E state = MANAGER_SM_COUNT_DOWN;
    int8_t countDown = 3;  // STATIC ASERT not bigger than 3
    uint8_t flickers = 16; // STATIC ASSERT even. //4
    int8_t meduzCountDown = getRandomNumber(5, 7);
    uint32_t score;
    int8_t lives = 1;

    GameShapes *GameShapes = GameShapes::getGameShapes(); // ALONB - change caps on actual obj

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
                GameShapes->setLives(lives);
                GameShapes->setActiveGame(lives);
                score = 0;
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
                lives--;
                state = MANAGER_SM_COLLISION;
                // cout << "[Manager] - collision detected " << GameShapes->getobsticals().size() << endl;
            }
            else
            {
                GameShapes->createNewShark();
                cout << "SHARK!" << endl;

                if (meduzCountDown-- <= 0)
                {
                    GameShapes->createNewMeduz();
                    meduzCountDown = getRandomNumber(1, 2);
                }

                GameShapes->cleanUpOldObjects();

                score += (10 / MANAGER_INITIAL_SHARKS_PER_SEC);
                GameShapes->updateScore(to_string(score));
                // cout << "OUT" << score << endl;
                // cout << "OUT" << to_string(score) << endl;

                // cout << "Num of obsticles is: " << GameShapes->getobsticals().size() << endl;
                this_thread::sleep_for(chrono::milliseconds(static_cast<uint32_t>(1000 / MANAGER_INITIAL_SHARKS_PER_SEC))); // ALONB - randomize this a bit? or make different sizes for octs..
            }
        }
        break;

        case MANAGER_SM_COLLISION:
        {
            // cout << "COLLLLLLLLL" << endl;
            GameShapes->flickerScreen();

            // std::cout << "[Manager] - New shape" << std::endl;
            this_thread::sleep_for(chrono::milliseconds(150));
            flickers--;
            if (flickers <= 0)
            {
                cout << "LIVES : " << lives << endl;
                if (lives > 0) // Because lives is not 0 based, 2 --> 2 lives, 1 means 1 life, 0 means no lifes.
                {
                    GameShapes->setActiveGame(lives);
                    state = MANAGER_SM_GAME;
                }
                else
                {
                    // cout << "GAME OVER : " << lives << endl;
                    GameShapes->gameOver(score, false);

                    state = MANAGER_SM_GAME_OVER;
                }
                flickers = 4;
            }
        }
        break;

        case MANAGER_SM_GAME_OVER:
        {
            // cout << "GAME OVER" << endl;

            auto timeoutDuration = std::chrono::seconds(3);

            // Wait for the result or timeout
            if (futureObj.wait_for(timeoutDuration) == std::future_status::ready)
            {
                // If the future is ready within the timeout, retrieve the result
                bool result = futureObj.get();
                std::cout << "Received data: " << std::endl;
                state = MANAGER_SM_ERROR; // New game
            }
            else
            {
                // If the future is not ready within the timeout, handle timeout
                std::cout << "Operation timed out!" << std::endl;
            }

            // std::this_thread::sleep_for(std::chrono::seconds(2));
            // this_thread::sleep_for(chrono::milliseconds(150));
        }

        case MANAGER_SM_ERROR:
        {
            std::cout << "ERROR" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
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