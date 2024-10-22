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

class simpleObjectFactory
{
private:
    static uint32_t numHeightPixels;
    static uint32_t numWidthPixels;

public:
    static list<unique_ptr<sf::RectangleShape>> createNum1();
    static list<unique_ptr<sf::RectangleShape>> createNum2();
    static list<unique_ptr<sf::RectangleShape>> createNum3();
};

class GeneralText
{
public:
    GeneralText(string displayString, float charSize, bool isBlink = false)
    {
        this->isBlink = isBlink;
        string filePath = getPathForAsset("RowsOfSunflowers", ".ttf");
        if (!font.loadFromFile(filePath))
        {
            std::cerr << "Error loading font!" << std::endl;
            return; // Exit if the image fails to load
        }
        text.setString(displayString);
        text.setFont(font);
        text.setCharacterSize(charSize);
        text.setFillColor(sf::Color::Red);
    }

    sf::Drawable *getDrawable() // ALONB - mayve if this has a drawable base, this is not needed!!! just put the object in tht pointer
    {
        return &text;
    }

    sf::FloatRect getBounds()
    {
        return text.getGlobalBounds();
    }

    bool getIsBlink()
    {
        return isBlink;
    }

    void setPosition(float xLocation, float yLocation) // 0.9, 0.25
    {
        text.setPosition(xLocation, yLocation);
    }

protected:
    sf::Text text;
    sf::Font font;
    bool isBlink;
};

class ScoreText : public GeneralText
{
public:
    ScoreText() : GeneralText("", dimensions::activeGameYOffset / 2)
    {
        setPosition(dimensions::screenDimentions.x * 0.9, dimensions::activeGameYOffset * 0.2); // ALONB - CHANGE the positioning to be relative?
    }

    void updateText(string str)
    {
        text.setString(str);
        auto rect = text.getGlobalBounds();
    }
};

class GameOverText : public GeneralText
{
public:
    GameOverText() : GeneralText("Game Over", dimensions::screenDimentions.y / 4)
    {
        setPosition((dimensions::screenDimentions.x - getBounds().width) / 2, (0));
    }
};

class ScoresText : public GeneralText
{
private:
    string synthasizeText(uint32_t currentScore, uint32_t highScore)
    {
        if (currentScore < highScore)
        {
            return "Good Try! high score is " + to_string(highScore);
        }

        else
        {
            return "High score reached - " + to_string(highScore) + "!";
        }
    }

public:
    ScoresText(float yPosition, uint32_t currentScore, uint32_t highScore) : GeneralText(synthasizeText(currentScore, highScore), dimensions::screenDimentions.y / 6)
    {
        setPosition((dimensions::screenDimentions.x - getBounds().width) / 2, yPosition);
        // setPosition((dimensions::screenDimentions.x - getBounds().width) / 2, 500);
    }
};

class pressEnterToRestart : public GeneralText
{
public:
    pressEnterToRestart() : GeneralText("Press Enter To Play", dimensions::screenDimentions.y / 4, true)
    {
        // setPosition((dimensions::screenDimentions.x - getBounds().width) / 2, dimensions::screenDimentions.y - getBounds().height);
        setPosition((dimensions::screenDimentions.x - getBounds().width) / 2, dimensions::screenDimentions.y - text.getCharacterSize() * 1.5);
    }
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

    // ALONB move all  this alpha stuff to chainging sprite
    void setLocation(float x, float y) { sprite.setPosition(x, y); }
    uint8_t getAlpha() { return sprite.getColor().a; }
    void fade(uint8_t units) { setAlpha(getAlpha() - units); }
    void setAlpha(uint8_t alpha)
    {
        sprite.setColor(sf::Color(OBJECTS_MAX_COLOR_VALUE, OBJECTS_MAX_COLOR_VALUE, OBJECTS_MAX_COLOR_VALUE, static_cast<sf::Uint8>(alpha)));
    }
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

class Prize : public RegularSprite
{
private:
    float fadeSpeedAlphaPerSec = 60;
    float alpha = 255;
    float yMid;
    float yDelta = 200;
    float max = 0;
    float min = 0;
    float timeBeforeFadingSec;
    float fadeTimeInSec;
    float fadeTimeConst; // ALONB - make this const
    bool fading;
    bool canReleaseFlag = false;

public:
    Prize();
    void advance(float dt, int8_t x = 1, int8_t y = 1)
    {
        // fade
        if (!fading)
        {
            timeBeforeFadingSec -= dt;
            if (timeBeforeFadingSec < 0)
            {
                cout << "START FADE" << endl;
                fadeTimeConst = fadeTimeInSec;
                fading = true;
            }
        }

        else if (!canReleaseFlag)
        {
            alpha = (fadeTimeInSec / fadeTimeConst) * 255;
            setAlpha(alpha);
            fadeTimeInSec -= dt;

            if (fadeTimeInSec < 0)
            {
                canReleaseFlag = true;
                sprite.setPosition(-1000, -1000); // so it gets clean by the collector.
            }
        }

        // move
        yDelta += (yMid - getBounds().top) * dt * 0.003;
        // cout << yDelta << endl;

        if ((getBounds().top < yMid) && (getBounds().top + yDelta >= yMid))
        {
            // Fix so that the boyancy movement stays in ranges.
            yDelta = 0.015;
        }
        sprite.move(0, yDelta);
    }

    bool canRelease()
    {
        return canReleaseFlag;
    }
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
    void advance(float dt, int8_t x = 1, int8_t y = 1) override
    {
        // cout << currentXSpeed << endl;
        // cout << "[x,y]" << (uint32_t)x << " " << (uint32_t)y << endl;
        accelarationX = 4000 * dt;
        accelarationY = 3000 * dt / 1.6;
        if (x == 1)
        {
            currentXSpeed = min(currentXSpeed + accelarationX, speedPixPerSecond.x);
        }

        else if (x == -1)
        {
            currentXSpeed = max(currentXSpeed - accelarationX, -speedPixPerSecond.x);
        }

        else if (x == 0)
        {
            if (currentXSpeed > 0)
            {
                currentXSpeed = max(currentXSpeed - accelarationX, 0.0f);
            }
            else if (currentXSpeed < 0)
            {
                currentXSpeed = min(currentXSpeed + accelarationX, 0.0f);
            }
        }

        if (y == 1)
        {
            currentYSpeed = min(currentYSpeed + accelarationY, speedPixPerSecond.y);
        }

        else if (y == -1)
        {
            currentYSpeed = max(currentYSpeed - accelarationY, -speedPixPerSecond.y);
        }

        else if (y == 0)
        {
            if (currentYSpeed > 0)
            {
                currentYSpeed = max(currentYSpeed - accelarationY, 0.0f);
            }
            else if (currentYSpeed < 0)
            {
                currentYSpeed = min(currentYSpeed + accelarationY, 0.0f);
            }
        }

        float dx = dt * currentXSpeed;
        float dy = dt * currentYSpeed;

        float actualX = clamp(this->getBounds().left + dx,
                              0.0f,
                              dimensions::activeGameDimentions.x - this->getBounds().width);

        if (actualX == 0.0f || actualX == dimensions::activeGameDimentions.x - this->getBounds().width)
        {
            currentXSpeed = 0;
        }
        float actualY = clamp(this->getBounds().top + dy,
                              static_cast<float>(dimensions::activeGameYOffset),
                              dimensions::screenDimentions.y - this->getBounds().height);

        if (actualY == static_cast<float>(dimensions::activeGameYOffset) || actualY == dimensions::screenDimentions.y - this->getBounds().height)
        {
            currentYSpeed = 0;
        }
        sprite.setPosition(actualX, actualY); // ALONB - try to do this with a move, more elegant.
    }
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
    sf::Drawable *getDrawable() // ALONB make this common function that all inherit. {}
    {
        return &item;
    }

    bool getIsBlink()
    {
        return false;
    }
};

// ALONB - provide a colliding function that takes 2 base objects?