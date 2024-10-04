#pragma once
#include <SFML/Graphics.hpp>
#include <definitions.h>
#include <memory>
#include <list>
#include <future>

// ALONB - seperate file for this?
class dB // ALONB Add static variable to make sure 1 initialization?
{
    // can be used for fast access to needed data
private:
    int32_t score;
    int32_t lives;
    uint8_t maxLives = MAX_LIVES;

    static unique_ptr<dB> instance;
    std::mutex _mutex2; // Define the mutex //ALONB MOVE THIS TO THE DB?

public:
    dB() : score(0), lives(0)
    {
        score = 0;
        lives = NUM_OF_INITIAL_LIVES;
        maxLives = MAX_LIVES;
    };

    static dB *getDB()
    {

        if (instance == nullptr)
        {
            instance = make_unique<dB>();
        }
        return instance.get();
    }

    uint32_t getScore()
    {
        std::lock_guard<std::mutex> lock(_mutex2);

        return score;
    }

    void setScore(uint32_t score)
    {
        std::lock_guard<std::mutex> lock(_mutex2);
        this->score = score;
    }

    void incrementScore(uint32_t increment)
    {
        std::lock_guard<std::mutex> lock(_mutex2);

        int32_t oldScore;

        oldScore = this->score;
        this->score += increment;

        uint8_t livesToinc = (this->score / SCORE_PER_EXTRA_LIFE) - (oldScore / SCORE_PER_EXTRA_LIFE);
        this->lives = min(this->lives + livesToinc, MAX_LIVES);
        // ALONB - lives is not protected properly.

        cout << "LIVES TO INC " << (uint32_t)livesToinc << endl;
    }

    uint32_t getLives()
    {
        std::lock_guard<std::mutex> lock(_mutex2);

        return lives;
    }

    void setLives(uint32_t lives)
    {
        std::lock_guard<std::mutex> lock(_mutex2);

        this->lives = lives;
    }

    void decLives()
    {
        std::lock_guard<std::mutex> lock(_mutex2);

        this->lives--;
    }

    uint8_t getMaxLives()
    {
        return maxLives;
    }
};
