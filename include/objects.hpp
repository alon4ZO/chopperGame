#pragma once
#include <SFML/Graphics.hpp>
#include <definitions.h>
#include <dimensions.hpp>
#include <utils.hpp>
#include <memory>
#include <list>
#include <mutex>
#include <algorithm>
#include <cassert>

// ALONB - use a font that renders better than the one i have now, maybe then the hight ratios will work better.
using namespace std;
using namespace std::literals;

// Texts
class GeneralText
{
private:
    sf::Font font;
    bool isBlink;

protected:
    sf::Text text;
    void setPosition(float xLocation, float yLocation) { text.setPosition(xLocation, yLocation); }

public:
    GeneralText(string displayString, float charSize, bool isBlink = false);
    sf::Drawable *getDrawable() { return &text; }
    sf::FloatRect getBounds() { return text.getGlobalBounds(); }
    bool getIsBlink() { return isBlink; }
};

class ScoreText : public GeneralText
{
public:
    ScoreText(float xPos);
    void updateText(string str) { text.setString(str); }
};

class GameOverText : public GeneralText
{
public:
    GameOverText();
};

class ScoresText : public GeneralText
{
private:
    string synthasizeText(uint32_t currentScore, uint32_t highScore);

public:
    ScoresText(float yPosition, uint32_t currentScore, uint32_t highScore);
};

class pressEnterToRestart : public GeneralText
{
public:
    pressEnterToRestart();
};

// sprites
class RegularSprite
{
public:
    RegularSprite(string filePath, float scaleX, float scaleYoverride = -1);
    sf::FloatRect getBounds() { return sprite.getGlobalBounds(); }
    bool getIsBlink() { return false; }
    bool checkColision(sf::FloatRect rectangle) { return this->getBounds().intersects(rectangle); }
    sf::Drawable *getDrawable() { return &sprite; } // make a drawable object classes that is inherited by texts, simple object, and regular sprite

protected:
    sf::Sprite sprite;
    sf::Texture texture;
    void setLocation(float x, float y) { sprite.setPosition(x, y); }
};

class lifeIcon : public RegularSprite
{
public:
    lifeIcon(uint8_t id);
};

class ScoresIcon : public RegularSprite
{
public:
    ScoresIcon();
};

class ChangingSprite : public RegularSprite
{
private:
    float timeUntilFadeStartSec;
    float fadingTimeSec;
    float remainingFadeTimeSec;
    int32_t shouldFade;

    void setAlpha(uint8_t alpha)
    {
        sprite.setColor(sf::Color(OBJECTS_MAX_COLOR_VALUE,
                                  OBJECTS_MAX_COLOR_VALUE,
                                  OBJECTS_MAX_COLOR_VALUE,
                                  static_cast<sf::Uint8>(alpha)));
    }

protected:
    sf::Vector2f speedPixPerSecond;

public:
    ChangingSprite(string filePath, float scale, sf::Vector2f speedScreenPerSec, bool shouldFade = false, int32_t timeUntilFadeStartSec = 0, int32_t fadeSpeedAlphaPerSec = 0);
    virtual void advance(float dt, int8_t x = 1, int8_t y = 1);
};

class Meduz : public ChangingSprite
{
public:
    Meduz(float scale, float verticleSpeed);
    void advance(float dt, int8_t x = 1, int8_t y = 1) override;
};

class Shark : public ChangingSprite
{
public:
    Shark(float scale, float horizontalSpeed);
};

class Bubble : public ChangingSprite
{
public:
    Bubble(sf::FloatRect playerBoundsRect);
};

class ExtraLifeIcon : public ChangingSprite
{
public:
    ExtraLifeIcon();
};

class Prize : public ChangingSprite
{
private:
    float yMid;
    float yDelta = 200;

public:
    Prize();
    void advance(float dt, int8_t x = 1, int8_t y = 1) override;
};

class Player : public ChangingSprite
{
private:
    float currentXSpeed;
    float currentYSpeed;
    float accelarationX;
    float accelarationY;

public:
    Player();
    void advance(float dt, int8_t x, int8_t y) override;
};

class CountDownNumberItem
{
private:
    sf::RectangleShape item;

public:
    CountDownNumberItem(sf::RectangleShape item)
    {
        this->item = item;
    }
    sf::Drawable *getDrawable()
    {
        return &item;
    }

    bool getIsBlink()
    {
        return false;
    }
};

class simpleObjectFactory
{
public:
    static list<unique_ptr<sf::RectangleShape>> createNum1();
    static list<unique_ptr<sf::RectangleShape>> createNum2();
    static list<unique_ptr<sf::RectangleShape>> createNum3();
};

// ALONB - provide a colliding function that takes 2 base objects?
// ALONB - make meduz decelerate when switching direction