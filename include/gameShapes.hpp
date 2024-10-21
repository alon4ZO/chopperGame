#pragma once
#include <SFML/Graphics.hpp>
#include <objects.hpp>
#include <utils.hpp>
#include <definitions.h>
#include <memory>
#include <list>
#include <mutex>

using namespace std;

// Create a random number generator and seed it with a random device

class GameShapes
{
private:
    static unique_ptr<GameShapes> instance;

    // Game objects:
    list<unique_ptr<sf::RectangleShape>> numCountdown;
    list<unique_ptr<RegularSprite>> lives;
    unique_ptr<RegularSprite> scoreSign;
    unique_ptr<ScoreText> score;
    list<unique_ptr<GeneralText>> gameOverTexts;
    unique_ptr<Player> player;
    list<unique_ptr<Shark>> sharks;
    list<unique_ptr<Meduz>> meduzes;
    list<unique_ptr<Bubble>> bubbles;
    list<unique_ptr<Prize>> prizes;
    list<unique_ptr<ExtraLifeIcon>> extraLifeIcons;

    // data:
    uint32_t highScore; // ALONB this shouldn't be here
    uint8_t livesToDraw;
    uint32_t blackout;
    bool isBlink;
    bool isGameOver;
    bool isCollisions;
    float TimeUntilBubble;
    float nextTimeUntilBubble;
    vector<sf::Drawable *> drawablesList;

    // functions
    void createNewBubble(float scale);
    template <typename T>
    void addToScreenShapeCollection(list<unique_ptr<T>> &collection, int8_t numberOfItemsToDraw = -1);
    void addToScreenSingleShape(float scale);

public:
    unique_ptr<AsyncSignal> asyncSignal;
    GameShapes() {};
    GameShapes operator=(GameShapes &) = delete;
    GameShapes(GameShapes &) = delete;
    static GameShapes *getGameShapes();

    std::mutex _mutex; // Some others modules must use this as well.

    void clearAll();
    void setActiveGame();
    void setCountDown(uint8_t num);
    void gameOver(uint32_t score, bool isHighScore);
    void blink();
    void resetAsyncSignal();
    void resetGameOver();
    void setLives();
    void createNewLiveIcon();
    void setGameScreenDimensions(uint32_t x, uint32_t y);
    void updateMovables(float dt, pair<int8_t, int8_t> playerSteps);
    void updateScore(string score);
    void createNewShark();
    void createNewMeduz();
    void createNewPrize();
    void cleanUpOldObjects();
    void checkCollisions();
    bool isCollionWithObsticle();
    bool getIsGameOver();
    void flickerScreen();
    vector<sf::Drawable *> &updateAndGetItemsToDraw();
};