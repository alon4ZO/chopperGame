#pragma once
#include <SFML/Graphics.hpp>
#include <definitions.h>
#include <dimensions.hpp>
#include <memory>
#include <list>
#include <mutex>
#include <algorithm>

using namespace std;

class shapeFactory
{
private:
    static uint32_t numHeightPixels;
    static uint32_t numWidthPixels;

public:
    static list<unique_ptr<sf::RectangleShape>> createNum1(uint32_t boardWidth, uint32_t boardHeight);
    static list<unique_ptr<sf::RectangleShape>> createNum2(uint32_t boardWidth, uint32_t boardHeight);
    static list<unique_ptr<sf::RectangleShape>> createNum3(uint32_t boardWidth, uint32_t boardHeight); // ALONB - this should be
    static string getPathForPng(string fileName, string postfix, uint8_t randomOptions = 1);           // Put this with the random num generator in utils tab.
};

class GameText
{
public:
    // GameText(scale, xPosition)
    GameText()
    {

        string filePath = shapeFactory::getPathForPng("RowsOfSunflowers", ".ttf");
        if (!font.loadFromFile(filePath))
        {
            std::cerr << "Error loading font!" << std::endl;
            return; // Exit if the image fails to load
        }
        text.setString("");
        text.setFont(font);
        text.setCharacterSize(dimensions::activeGameYOffset / 2);
        text.setFillColor(sf::Color::Red);

        // sf::FloatRect originalSize(text.getGlobalBounds());

        // float desiredHeight = dimensions::activeGameYOffset * 0.5; // Example desired width in pixels
        // float imageScale = desiredHeight / originalSize.height;
        // text.setScale(imageScale, imageScale);

        // text.setScale(float factorX, float factorY)

        // text.setPosition(dimensions::activeGameDimentions.x / 2, dimensions::activeGameDimentions.y / 2);
        text.setPosition(dimensions::screenDimentions.x * 0.9, (dimensions::activeGameYOffset / 4));
        // text.setPosition(dimensions::screenDimentions.x * 0.8, 0);
        // set the position of the object

        // void 	setScale (const Vector2f &factors)
    }

    void updateText(string str)
    {
        text.setString(str);
        auto rect = text.getGlobalBounds();
        // cout << rect.left << " " << rect.top << " " << rect.width << " " << rect.height << endl;
    }

    sf::Drawable *getDrawable() // ALONB - mayve if this has a drawable base, this is not needed!!! just put the object in tht pointer
    {
        return &text;
    }

    sf::FloatRect getBounds()
    {
        return text.getGlobalBounds();
    }

private:
    sf::Text text;
    sf::Font font;
};

class RegularSprite
{
public:
    RegularSprite(string filePath, float scale, float scaleYoverride = -1) // ALONB - game screen dimensions should be retrieved every time or just saved? maybe it's a problem because the speed is also based on the screen dimentions and is set far above...
    // ALONB - scale is according to X
    {
        // cout << "Making S " << filePath << "scale: " << scale << "x,y" << dimensions::screenDimentions.x << " " << dimensions::screenDimentions.y << endl;
        if (!texture.loadFromFile(filePath))
        {
            std::cerr << "Error loading image!" << std::endl;
            return; // Exit if the image fails to load
        }
        sf::Vector2f originalSize(texture.getSize());

        // Calculate scale factors

        float imageScale;

        if (scaleYoverride == -1)
        {
            float desiredWidth = dimensions::activeGameDimentions.x * scale; // Example desired width in pixels
            imageScale = desiredWidth / originalSize.x;
        }
        else
        {
            // cout << dimensions::screenDimentions.y << endl;
            // cout << scaleYoverride << endl;
            // cout << originalSize.y << endl;
            float desiredHeight = dimensions::screenDimentions.y * scaleYoverride; // Example desired width in pixels

            imageScale = desiredHeight / originalSize.y;
            cout << "D" << endl;
            cout << imageScale << endl;
            cout << originalSize.y << endl;
            cout << (originalSize.y * imageScale) << endl;
            cout << "S" << endl;
        }

        // sprite.setPosition(500, 500);
        // Create a sprite from the texture
        // cout << "imageScale " << imageScale << endl;
        sprite.setTexture(texture);
        //  sprite.setPosition(locationPix.x, locationPix.y);
        sprite.setScale(imageScale, imageScale);

        if (scaleYoverride != -1)
        {
            cout << "0HEIGHT:" << sprite.getGlobalBounds().height << endl;
            cout << "WALL:" << dimensions::activeGameYOffset << endl;
        }
    }

    sf::FloatRect getBounds()
    {
        return sprite.getGlobalBounds();
    }

    void setLocation(float x, float y) // This is not really needed no?
    {
        sprite.setPosition(x, y);
    }

    sf::Drawable *getDrawable() // ALONB - mayve if this has a drawable base, this is not needed!!! just put the object in tht pointer
    {
        return &sprite;
    }

protected:
    sf::Sprite sprite;
    sf::Texture texture;
};

class lifeIcon : public RegularSprite
{
public:
    lifeIcon(uint8_t id) : RegularSprite(shapeFactory::getPathForPng("player", ".png"), 0, GAME_BOARD_WALL_WIDTH_RATIO * 0.3)
    // lifeIcon(uint8_t id) : RegularSprite(shapeFactory::getPathForPng("player", ".png"), 0, 0.1)
    {
        setLocation(dimensions::screenDimentions.x * 0.03 + (getBounds().width + dimensions::screenDimentions.x * 0.03) * id,
                    (dimensions::activeGameYOffset - getBounds().height) / 2);
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
    bool checkColision(sf::FloatRect rectangle)
    {
        return this->getBounds().intersects(rectangle);
    }
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

class Bubble : public MovingSprite
{
public:
    Bubble(float scale, float verticleSpeed);
    void advance(float dt, int8_t x = 1, int8_t y = 1) {} // add some sideways motion as well?
};

class Player : public MovingSprite
{
public:
    Player(float scale);
    void advance(float dt, int8_t x = 1, int8_t y = 1)
    {

        float dx = dt * speedPixPerSecond.x * x; // ALONB - this turns out to be a bit wasteful...
        float dy = dt * speedPixPerSecond.y * y;

        // This has to be calculated with the post move, otherwise it gets stuck!
        sf::FloatRect screenRect(this->getBounds().width, dimensions::activeGameYOffset + this->getBounds().height, dimensions::activeGameDimentions.x - 2 * this->getBounds().width, dimensions::activeGameDimentions.y - 2 * this->getBounds().height);

        dx = dt * speedPixPerSecond.x * x;
        dy = 0;
        {

            sf::FloatRect playerPostMoveRect(this->getBounds().left + dx, this->getBounds().top + dy, this->getBounds().width, this->getBounds().height);

            if (screenRect.intersects(playerPostMoveRect))
            {
                sprite.move(dx, 0);
            }
        }

        dx = 0;
        dy = dt * speedPixPerSecond.y * y;
        {
            sf::FloatRect playerPostMoveRect(this->getBounds().left + dx, this->getBounds().top + dy, this->getBounds().width, this->getBounds().height);

            if (screenRect.intersects(playerPostMoveRect))
            {
                sprite.move(0, dy);
            }
        }
    }
};
