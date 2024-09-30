#pragma once
#include <SFML/Graphics.hpp>
#include <objects.hpp>
#include <definitions.h>
#include <memory>
#include <list>
#include <mutex>
#include <algorithm>
#include <future>

using namespace std;

class AsyncSignal
{
private:
    // std::mutex _SignalMutex; // Define the mutex
    std::promise<bool> promiseObj;
    std::future<bool> futureObj;

public:
    AsyncSignal()
    {
        futureObj = promiseObj.get_future();
    }

    void send()
    {
        promiseObj.set_value(true);
    }

    bool recieve(uint32_t timeInMs)
    {
        auto timeoutDuration = std::chrono::milliseconds(850);
        return (futureObj.wait_for(timeoutDuration) == std::future_status::ready);
    }
};

class GameShapes
{
private:
    static unique_ptr<GameShapes> instance;

    uint32_t blackout;
    bool isBlink;

    unique_ptr<Player> player;
    list<unique_ptr<Shark>> sharks;
    list<unique_ptr<Meduz>> meduzes;
    list<unique_ptr<Bubble>> bubbles;
    list<unique_ptr<sf::RectangleShape>> numCountdown;
    unique_ptr<ScoreText> score;
    unique_ptr<RegularSprite> scoreSign; // ALONB - maybe some of these should be static
    list<unique_ptr<RegularSprite>> lives;
    list<unique_ptr<GeneralText>> gameOverTexts;
    list<unique_ptr<Prize>> prizes;

    uint8_t livesToDraw;
    uint32_t highScore;
    bool isGameOver; // Init?

    float TimeUntilBubble;
    float nextTimeUntilBubble;

    // Draw it

    // vector<bubbles> score;

    // All Items to draw
    // bool is - isLlist
    vector<sf::Drawable *> drawablesList;

    pair<bool, bool> isCollisions;

    void createObsticle(uint32_t xPos, uint32_t boardHeight, uint32_t wallThickness, uint32_t spacing);
    // unique_ptr<sf::Shape> shapeFactory();

    float clamp(float value, float minVal, float maxVal) // Just in case CPP is older than 17 used explicit function.
    {
        return max(minVal, min(maxVal, value));
    }
    void createNewBubble(float scale);

public:
    unique_ptr<AsyncSignal> asyncSignal;
    // GameShapes(uint32_t x, uint32_t y);
    GameShapes()
    {
        blackout = false;
        // TimeUntilBubble = 1.0f;
    }
    GameShapes operator=(GameShapes &) = delete;
    GameShapes(GameShapes &) = delete;
    static GameShapes *getGameShapes();

    void resetAsyncSignal();

    std::mutex _mutex; // Define the mutex

    // void clearAll(); // ALONB - make all these list part of a vector for safer cleaning.
    void clearAll();
    void setActiveGame(uint8_t lives);

    void setCountDown(uint8_t num);

    void gameOver(uint32_t score, bool isHighScore);

    void resetGameOver();

    void setLives(uint8_t num);

    void setGameScreenDimensions(uint32_t x, uint32_t y)
    {

        dimensions::screenDimentions.x = x;
        dimensions::screenDimentions.y = y;

        dimensions::activeGameYOffset = y * GAME_BOARD_WALL_WIDTH_RATIO;

        dimensions::activeGameDimentions.x = x;
        dimensions::activeGameDimentions.y = dimensions::screenDimentions.y - dimensions::activeGameYOffset;
    }

    vector<sf::Drawable *> &updateAndGetItemsToDraw();

    // getItemsToDraw
    // const list<unique_ptr<sf::Shape>> &getWalls() const { return walls; }
    // const list<unique_ptr<sf::Shape>> &getchopper() const { return chopper; }
    // const list<unique_ptr<sf::Shape>> &getobsticals() const { return obsticals; }
    // const list<unique_ptr<sf::Shape>> &getCountDown() const { return countDown; }
    // const list<unique_ptr<sf::Shape>> &emptyBlack() const { return blackout; } // ALONB - instead of putting this on top, I should really just not preset anything.
    void
    updateMovables(float dt, pair<int8_t, int8_t> playerSteps);

    void updateScore(string score);

    void createNewShark();
    void createNewMeduz();
    void createNewPrize();
    void cleanUpOldObjects();
    void moveChopper(pair<float, float> direction) // ALONB - move this from here.
    {
        // for (auto &i : chopper)
        // {
        //     i->move(direction.first, 0.0f);
        //     i->move(0.0f, direction.second);

        //     auto bounds = i->getGlobalBounds();
        //     sf::Vector2f position;

        //     position.x = clamp(bounds.left, 0, screenDimentions.x - bounds.width);
        //     position.y = clamp(bounds.top, wallThickness, screenDimentions.y - bounds.height);

        //     i->setPosition(position);
        // }
    }

    void checkCollisions();
    bool isCollionWithObsticle() { return isCollisions.first; }
    bool getIsGameOver()
    {
        return isGameOver;
    }
    void flickerScreen();
    void blink();

    // first is obstible and second is life
};

// ALONB make see with waves on top