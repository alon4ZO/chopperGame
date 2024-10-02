#include <SFML/Graphics.hpp>
#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <manager.hpp>
#include <dB.hpp>
#include <thread>
#include <chrono>
#include <future>

using namespace std;
using namespace std::literals;

unique_ptr<dB> dB::instance = nullptr;

typedef enum ManagerSm
{
    MANAGER_SM_RESET = 0,
    MANAGER_SM_COUNT_DOWN,
    MANAGER_SM_GAME,
    MANAGER_SM_COLLISION,
    MANAGER_SM_GAME_OVER,
    MANAGER_SM_ERROR
} MANAGER_SM_E;

void Manager::Start(std::future<bool> &&futureObj)
{
    MANAGER_SM_E state = MANAGER_SM_RESET;
    int8_t countDown; // STATIC ASERT not bigger than 3
    uint8_t flickers; // STATIC ASSERT even. //4
    int16_t meduzCountDown;
    int16_t prizeCountDown;

    GameShapes *GameShapes = GameShapes::getGameShapes(); // ALONB - change caps on actual obj
    dB *dB = dB::getDB();

    // GameShapes->setActiveGame();
    while (1)
    {
        // std::cout << "R" << std::endl;

        switch (state)
        {

        case MANAGER_SM_RESET:
        {
            std::cout << "[Manager] - reseting " << std::endl;

            countDown = 3; // STATIC ASERT not bigger than 3
            flickers = 4;  // STATIC ASSERT even. //4
            meduzCountDown = getRandomNumber(5000, 50000);
            // prizeCountDown = getRandomNumber(1, 2);
            prizeCountDown = 2;
            dB->setScore(0);
            dB->setLives(2);

            GameShapes->clearAll();
            state = MANAGER_SM_COUNT_DOWN;
        }
        break;

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
                GameShapes->setLives(); // If using the DB, in both cases don't need to send the lives.
                GameShapes->setActiveGame(dB->getLives());
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
                dB->decLives();
                state = MANAGER_SM_COLLISION;
                // cout << "[Manager] - collision detected " << GameShapes->getobsticals().size() << endl;
            }
            else
            {
                GameShapes->createNewShark();
                // cout << "SHARK!" << endl;

                if (meduzCountDown-- <= 0)
                {
                    GameShapes->createNewMeduz();
                    meduzCountDown = getRandomNumber(5000, 6000);
                }

                if (prizeCountDown-- <= 0)
                {
                    GameShapes->createNewPrize();
                    // prizeCountDown = getRandomNumber(1, 2);
                    prizeCountDown = 2;
                }

                GameShapes->cleanUpOldObjects();

                dB->incrementScore(10);
                GameShapes->updateScore(to_string(dB->getScore()));                                                         // ALONB - don't need to send this?
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
                // cout << "LIVES : " << lives << endl;
                if (dB->getLives() > 0) // Because lives is not 0 based, 2 --> 2 lives, 1 means 1 life, 0 means no lifes.
                {
                    GameShapes->setActiveGame(dB->getLives());
                    state = MANAGER_SM_GAME;
                }
                else
                {
                    // cout << "GAME OVER : " << lives << endl;
                    GameShapes->gameOver(dB->getScore(), false);

                    // reset the future and promise, update
                    // auto newptr = make_unique<AsyncSignal>();
                    // GameShapes->asyncSignal = move(newptr);
                    GameShapes->asyncSignal.reset(new AsyncSignal());
                    // GameShapes->asyncSignal.reset(move(make_unique<AsyncSignal>()));
                    GameShapes->clearAll();

                    state = MANAGER_SM_GAME_OVER;
                }
                flickers = 4;
            }
        }
        break;

        case MANAGER_SM_GAME_OVER:
        {
            // cout << "GAME OVER" << endl;

            // GameShapes->blink();
            if (GameShapes->asyncSignal->recieve(500))
            {
                state = MANAGER_SM_RESET;
            }
            else
            {
                GameShapes->blink();
            }
        }
        break;

        case MANAGER_SM_ERROR:
        {
            std::cout << "ERROR" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        break;
        }
    }
}
