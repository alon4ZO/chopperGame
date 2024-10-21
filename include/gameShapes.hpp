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
    // list<unique_ptr<sf::RectangleShape>> numCountdown;
    list<unique_ptr<CountDownNumberItem>> numCountdown;
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
    uint32_t blackout;
    bool isBlink;
    bool isGameOver;
    bool isCollisions;
    float TimeUntilBubble;
    float nextTimeUntilBubble;
    vector<sf::Drawable *> drawablesList;

    // functions
    template <typename T>
    void addShapeCollectionToScreen(list<unique_ptr<T>> &collection, int8_t numberOfItemsToDraw = -1);
    template <typename T>
    void addSingleShapeToScreen(T &item);
    template <typename T>
    void simpleMoveShapeCollection(list<unique_ptr<T>> &collection, float dt);

    void clearGameBoard();
    void clearEndOfGame();
    void createNewBubble();

public:
    unique_ptr<AsyncSignal> asyncSignal;
    GameShapes() {};
    GameShapes operator=(GameShapes &) = delete;
    GameShapes(GameShapes &) = delete;
    static GameShapes *getGameShapes();

    std::mutex _mutex; // Some others modules must use this as well.

    void setActiveGame();
    void setLives();
    void setCountDown(uint8_t num);
    bool getIsGameOver();
    void gameOver(uint32_t score, bool isHighScore);
    void resetGameOver();
    void flickerScreen();
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
    void blink();
    void resetAsyncSignal();

    vector<sf::Drawable *> &updateAndGetItemsToDraw();
};