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
    int32_t lives; // how many pixels per second
    static unique_ptr<dB> instance;
    std::mutex _mutex; // Define the mutex //ALONB MOVE THIS TO THE DB?

public:
    dB() : score(0), lives(0) {};

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
        std::lock_guard<std::mutex> lock(_mutex);

        return score;
    }

    void setScore(uint32_t score)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        this->score = score;
    }

    void incrementScore(uint32_t increment)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        this->score += increment;
    }

    uint32_t getLives()
    {
        std::lock_guard<std::mutex> lock(_mutex);

        return lives;
    }

    void setLives(uint32_t lives)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        this->lives = lives;
    }

    void decLives()
    {
        std::lock_guard<std::mutex> lock(_mutex);

        this->lives--;
    }
};
