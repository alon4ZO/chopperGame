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

        return score;
    }

    void resetScore()
    {
        this->score = 0;
    }

    bool incrementScore(uint32_t increment)
    {

        int32_t oldScore;

        oldScore = this->score;
        this->score += increment;

        bool isIncreaseLives = (this->score / SCORE_PER_EXTRA_LIFE) - (oldScore / SCORE_PER_EXTRA_LIFE) > 0 ? true : false;

        if (this->lives == MAX_LIVES)
        {
            return false;
        }
        this->lives += isIncreaseLives ? 1 : 0;
        return isIncreaseLives;
    }

    uint32_t getLives()
    {

        return lives;
    }

    void setLives(uint32_t lives)
    {

        this->lives = lives;
    }

    void decLives()
    {
        cout << "DEC" << endl;
        this->lives--;
    }

    uint8_t getMaxLives()
    {
        return maxLives;
    }
};
