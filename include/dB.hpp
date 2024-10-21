#pragma once
#include <SFML/Graphics.hpp>
#include <definitions.h>
#include <memory>
#include <list>
#include <future>

typedef enum dbIncPoints
{
    DB_INC_POINTS_SHARK = DB_SCORE_SCORE_FOR_NEW_SHARK,
    DB_INC_POINTS_MEDUZ = DB_SCORE_SCORE_FOR_NEW_MEDUZA,
    DB_INC_POINTS_EXTRA_LIFE = DB_SCORE_SCORE_FOR_EXTRA_LIFE_BONUS
} DB_INC_POINTS_E;

class dB // ALONB Add static variable to make sure 1 initialization?
{
    // can be used for fast access to needed data
private:
    int32_t score;
    int32_t highScore; // ALONB why is this signed?
    int32_t lives;
    uint8_t maxLives = DB_MAX_LIVES;

    static unique_ptr<dB> instance;

public:
    dB() : score(0), lives(0)
    {
        score = 0;
        highScore = 0;
        lives = DB_NUM_OF_INITIAL_LIVES;
        maxLives = DB_MAX_LIVES;
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

    uint32_t getHighScore()
    {

        return highScore;
    }

    void reset()
    {
        this->score = 0;
        this->lives = DB_NUM_OF_INITIAL_LIVES;
    }

    bool incrementScore(DB_INC_POINTS_E increment)
    {
        int32_t oldScore; // ALONB why is this signed?

        oldScore = this->score;
        this->score += increment;
        if (this->score >= highScore)
        {
            highScore = this->score;
        }

        bool isIncreaseLives = (this->score / DB_SCORE_PER_EXTRA_LIFE) - (oldScore / DB_SCORE_PER_EXTRA_LIFE) > 0 ? true : false;

        if (this->lives == DB_MAX_LIVES)
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
        this->lives--;
    }

    uint8_t getMaxLives()
    {
        return maxLives;
    }
};
