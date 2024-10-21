#include <iostream>
#include <gameShapes.hpp>
#include <manager.hpp>
#include <dB.hpp>
#include <thread>

using namespace std;
using namespace std::literals;

unique_ptr<dB> dB::instance = nullptr; // ALONB why is this here?

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
    float sharksPerSec;
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
            sharksPerSec = MANAGER_INITIAL_SHARKS_PER_SECOND;

            meduzCountDownMs = -1;
            prizeCountDownMs = getRandomNumber(
                GAME_MANAGER_PRIZE_COUNTDOWN_TIME_MS * (1.0f - GAME_MANAGER_GENERAL_RANDOM_FACTOR),
                GAME_MANAGER_PRIZE_COUNTDOWN_TIME_MS * (1.0f + GAME_MANAGER_GENERAL_RANDOM_FACTOR));

            {
                std::lock_guard<std::mutex> lock(GameShapes->_mutex);
                dBInst->reset();
            }

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
                GameShapes->setLives();
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

                    {
                        std::lock_guard<std::mutex> lock(GameShapes->_mutex);
                        if (dBInst->incrementScore(DB_SCORE_SCORE_FOR_NEW_MEDUZA))
                        {
                            GameShapes->createNewLiveIcon();
                        }
                        string scoreString = to_string(dBInst->getScore());
                        GameShapes->updateScore(scoreString);
                    }
                    meduzCountDownMs = getRandomNumber(
                        GAME_MANAGER_MEDUZA_COUNTDOWN_TIME_MS * (1.0f - GAME_MANAGER_GENERAL_RANDOM_FACTOR),
                        GAME_MANAGER_MEDUZA_COUNTDOWN_TIME_MS * (1.0f + GAME_MANAGER_GENERAL_RANDOM_FACTOR));
                }

                if (prizeCountDownMs <= 0)
                {
                    GameShapes->createNewPrize();
                    prizeCountDownMs = getRandomNumber(
                        GAME_MANAGER_PRIZE_COUNTDOWN_TIME_MS * (1.0f - GAME_MANAGER_GENERAL_RANDOM_FACTOR),
                        GAME_MANAGER_PRIZE_COUNTDOWN_TIME_MS * (1.0f + GAME_MANAGER_GENERAL_RANDOM_FACTOR));
                }

                GameShapes->cleanUpOldObjects();

                {
                    std::lock_guard<std::mutex> lock(GameShapes->_mutex);
                    if (dBInst->incrementScore(DB_SCORE_SCORE_FOR_NEW_SHARK))
                    {
                        GameShapes->createNewLiveIcon();
                    }
                    string scoreString = to_string(dBInst->getScore());
                    GameShapes->updateScore(scoreString);
                }

                uint32_t nextSleep = static_cast<uint32_t>(1000 / sharksPerSec);
                this_thread::sleep_for(chrono::milliseconds(nextSleep));

                meduzCountDownMs -= nextSleep;
                prizeCountDownMs -= nextSleep;
            }
        }
        break;

        case MANAGER_SM_COLLISION:
        {
            GameShapes->flickerScreen();

            this_thread::sleep_for(chrono::milliseconds(GAME_MANAGER_SHARKS_FLICKER_TIME_MS));
            flickers--;
            if (flickers <= 0)
            {

                {
                    std::lock_guard<std::mutex> lock(GameShapes->_mutex);
                    dBInst->decLives();
                }
                if (dBInst->getLives() > 0)
                {
                    GameShapes->setActiveGame();
                    flickers = GAME_MANAGER_FLICKERS_WHEN_COLLIDE;
                    state = MANAGER_SM_GAME;
                }
                else
                {
                    GameShapes->gameOver(dBInst->getScore(), false);
                    GameShapes->asyncSignal.reset(new AsyncSignal());
                    state = MANAGER_SM_GAME_OVER;
                }
            }
        }
        break;

        case MANAGER_SM_GAME_OVER:
        {
            std::cout << "[Manager] - game over " << std::endl;
            if (GameShapes->asyncSignal->recieve(GAME_OVER_BLINK_TIME_MS))
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
            std::cout << "[Manager] - error " << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        break;
        }
    }
}
