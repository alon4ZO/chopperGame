#include <SFML/Graphics.hpp>
#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <manager.hpp>
#include <dB.hpp>
#include <thread>
#include <chrono>
#include <future>
#include <type_traits>

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

Manager::Manager()
{
    assert(!isInitialized && "screen was already initilized");
    isInitialized = true;
};

void Manager::Start()
{
    MANAGER_SM_E state = MANAGER_SM_RESET;
    int8_t countDown;
    uint8_t flickers;
    int16_t meduzCountDownMs; // This is counted in seconds so that even when level rises, meduzes stay at constant rate.
    int16_t prizeCountDownMs;

    GameShapes *GameShapes = GameShapes::getGameShapes(); // ALONB - change caps on actual obj
    dB *dBInst = dB::getDB();

    while (1)
    {
        switch (state)
        {
        case MANAGER_SM_RESET:
        {
            std::cout << "[Manager] - reseting " << std::endl;

            countDown = GAME_MANAGER_COUNTDOWN_START_NUM;
            flickers = GAME_MANAGER_FLICKERS_WHEN_COLLIDE;
            meduzCountDownMs = -1;
            prizeCountDownMs = getRandomNumber(
                GAME_MANAGER_PRIZE_COUNTDOWN_TIME_MS * (1.0f - GAME_MANAGER_GENERAL_RANDOM_FACTOR),
                GAME_MANAGER_PRIZE_COUNTDOWN_TIME_MS * (1.0f + GAME_MANAGER_GENERAL_RANDOM_FACTOR));

            {
                std::lock_guard<std::mutex> lock(GameShapes->_mutex);
                dBInst->reset();
            }

            GameShapes->clearAll(); // ALONB - why this not clearing all?
            state = MANAGER_SM_COUNT_DOWN;
        }
        break;

        case MANAGER_SM_COUNT_DOWN:
        {
            std::cout << "[Manager] - counting down - " << static_cast<uint32_t>(countDown) << std::endl;
            GameShapes->setCountDown(countDown);
            countDown--;
            if (countDown < 0)
            {
                state = MANAGER_SM_GAME;
                std::cout << "[Manager] - Starting active game " << std::endl;
                GameShapes->setLives(); // If using the DB, in both cases don't need to send the lives.
                GameShapes->setActiveGame();
                continue;
            }
            this_thread::sleep_for(chrono::milliseconds(GAME_BOARD_COUNTDOWN_TIME_INTERVALS_MS));
        }
        break;

        case MANAGER_SM_GAME:
        {

            if (GameShapes->isCollionWithObsticle())
            {
                state = MANAGER_SM_COLLISION;
            }
            else
            {
                GameShapes->createNewShark();

                if (meduzCountDownMs <= 0)
                {
                    GameShapes->createNewMeduz();
                    meduzCountDownMs = getRandomNumber(
                        GAME_MANAGER_MEDUZA_COUNTDOWN_TIME_MS * (1.0f - GAME_MANAGER_GENERAL_RANDOM_FACTOR),
                        GAME_MANAGER_MEDUZA_COUNTDOWN_TIME_MS * (1.0f + GAME_MANAGER_GENERAL_RANDOM_FACTOR));
                }

                if (prizeCountDownMs <= 0)
                {
                    GameShapes->createNewPrize();
                    // prizeCountDown = getRandomNumber(1, 2);
                    prizeCountDownMs = getRandomNumber(
                        GAME_MANAGER_PRIZE_COUNTDOWN_TIME_MS * (1.0f - GAME_MANAGER_GENERAL_RANDOM_FACTOR),
                        GAME_MANAGER_PRIZE_COUNTDOWN_TIME_MS * (1.0f + GAME_MANAGER_GENERAL_RANDOM_FACTOR));
                }

                GameShapes->cleanUpOldObjects();
                string scoreString;
                {

                    std::lock_guard<std::mutex> lock(GameShapes->_mutex);
                    if (dBInst->incrementScore(10))
                    {
                        cout << "INC from manager" << endl;
                        GameShapes->createNewLiveIcon();
                    }
                    scoreString = to_string(dBInst->getScore());
                }
                GameShapes->updateScore(scoreString);                                                                       // ALONB - need to send this?
                this_thread::sleep_for(chrono::milliseconds(static_cast<uint32_t>(1000 / MANAGER_INITIAL_SHARKS_PER_SEC))); // ALONB - randomize this a bit? or make different sizes for octs..

                meduzCountDownMs -= static_cast<uint32_t>(1000 / MANAGER_INITIAL_SHARKS_PER_SEC);
                prizeCountDownMs -= static_cast<uint32_t>(1000 / MANAGER_INITIAL_SHARKS_PER_SEC);
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

                {
                    std::lock_guard<std::mutex> lock(GameShapes->_mutex);
                    dBInst->decLives();
                }
                // cout << "LIVES : " << lives << endl;
                if (dBInst->getLives() > 0) // Because lives is not 0 based, 2 --> 2 lives, 1 means 1 life, 0 means no lifes.
                {
                    cout << "TST" << endl;
                    GameShapes->setActiveGame();
                    state = MANAGER_SM_GAME;
                }
                else
                {
                    // cout << "GAME OVER : " << lives << endl;
                    GameShapes->gameOver(dBInst->getScore(), false);

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
