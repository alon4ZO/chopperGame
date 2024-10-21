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
    RegularSprite(string filePath, float scaleX, float scaleYoverride = -1)
    {
        if (!texture.loadFromFile(filePath))
        {
            std::cerr << "Error loading image!" << std::endl;
            assert(0); // ALONB - use exception?
        }
        sf::Vector2f originalSize(texture.getSize());

        // Calculate scale factors
        float imageScale;

        if (scaleYoverride == -1)
        {
            float desiredWidth = dimensions::screenDimentions.x * scaleX;
            imageScale = desiredWidth / originalSize.x;
        }
        else
        {
            float desiredHeight = dimensions::screenDimentions.y * scaleYoverride;
            imageScale = desiredHeight / originalSize.y;
        }

        setAlpha(OBJECTS_MAX_COLOR_VALUE);
        sprite.setTexture(texture);
        sprite.setScale(imageScale, imageScale);
    }

    sf::FloatRect getBounds()
    {
        return sprite.getGlobalBounds();
    }

    bool checkColision(sf::FloatRect rectangle)
    {
        return this->getBounds().intersects(rectangle);
    }

    sf::Drawable *getDrawable() // ALONB - mayve if this has a drawable base, this is not needed!!! just put the object in tht pointer
    {
        return &sprite;
    }

    bool getIsBlink()
    {
        return false;
    }

protected:
    sf::Sprite sprite;
    sf::Texture texture;

    void setLocation(float x, float y)
    {
        sprite.setPosition(x, y);
    }

    void setAlpha(uint8_t alpha)
    {
        sprite.setColor(sf::Color(OBJECTS_MAX_COLOR_VALUE, OBJECTS_MAX_COLOR_VALUE, OBJECTS_MAX_COLOR_VALUE, static_cast<sf::Uint8>(alpha)));
    }

    uint8_t getAlpha()
    {
        return sprite.getColor().a;
    }

    void fade(uint8_t units)
    {
        setAlpha(getAlpha() - units);
    }
};

class lifeIcon : public RegularSprite
{
public:
    lifeIcon(uint8_t id) : RegularSprite(getPathForAsset("player", ".png"), 0, GAME_SCREEN_WALL_WIDTH_RATIO * 0.3)
    // lifeIcon(uint8_t id) : RegularSprite(ObjectFactory::getPathForAssets("player", ".png"), 0, 0.1)
    {
        setLocation(dimensions::screenDimentions.x * 0.03 + (getBounds().width + dimensions::screenDimentions.x * 0.03) * id,
                    (dimensions::activeGameYOffset - getBounds().height) / 2);
    }
};

class ScoresIcon : public RegularSprite
{
public:
    ScoresIcon() : RegularSprite(getPathForAsset("score_sign", ".png"), 0.03)
    {
        setLocation(dimensions::activeGameDimentions.x / 2, (dimensions::activeGameYOffset * 1.1 - getBounds().height) / 2);
    }
};

class MovingSprite : public RegularSprite
{
protected: // should this be protected?
    sf::Vector2f speedPixPerSecond;

public:
    MovingSprite(string filePath, float scale, sf::Vector2f speed) : RegularSprite(filePath, scale), speedPixPerSecond({speed.x * dimensions::activeGameDimentions.x, speed.y * dimensions::activeGameDimentions.y}) {};
    virtual void advance(float dt, int8_t x = 1, int8_t y = 1) = 0;
    // IMPLEMENT THE DRAW HERE SO SCREEN CAN USE THIS ALONB

public:
};

class Obsticle : public MovingSprite
{
private:
    sf::FloatRect collisionRectangle;

public:
    ~Obsticle() {}
    Obsticle(string filePath, float scale, sf::Vector2f speed) : MovingSprite(filePath, scale, speed) {} // ALONB - optimize
};

class Meduz : public Obsticle
{
public:
    Meduz(float scale, float verticleSpeed);
    void advance(float dt, int8_t x = 1, int8_t y = 1)
    {
        float dy = dt * speedPixPerSecond.y * y;

        if ((this->getBounds().top + dy) < dimensions::activeGameYOffset)
        {
            speedPixPerSecond.y *= -1;
        }

        sprite.move(dt * speedPixPerSecond.x * x, dt * speedPixPerSecond.y * y);
    }
};

class Shark : public Obsticle
{
public:
    Shark(float scale, float horizontalSpeed);
    void advance(float dt, int8_t x = 1, int8_t y = 1)
    {
        sprite.move(dt * speedPixPerSecond.x * x, dt * speedPixPerSecond.y * y);
    }
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

class Bubble : public MovingSprite
{
public:
    Bubble(sf::FloatRect playerBoundsRect);
    void advance(float dt, int8_t x = 1, int8_t y = 1)
    {
        sprite.move(dt * speedPixPerSecond.x * x, dt * speedPixPerSecond.y * y);
    } // add some sideways motion as well?
};

class Player : public MovingSprite
{
private:
    float currentXSpeed;
    float currentYSpeed;
    float accelarationX;
    float accelarationY;

public:
    Player();
    void advance(float dt, int8_t x = 1, int8_t y = 1)
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
        sprite.setPosition(actualX, actualY);
    }
};

class ExtraLifeIcon : public MovingSprite
{
    float fadeTimeInSec;
    float fadeTimeConst;

public:
    ExtraLifeIcon();
    void advance(float dt, int8_t x = 1, int8_t y = 1)
    {
        sprite.move(dt * speedPixPerSecond.x * x, dt * speedPixPerSecond.y * y);

        if (fadeTimeInSec > 0)
        {
            float alpha = (fadeTimeInSec / fadeTimeConst) * 255;
            setAlpha(alpha);
            fadeTimeInSec -= dt;
        }
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
