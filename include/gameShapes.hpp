#pragma once
#include <SFML/Graphics.hpp>
#include <definitions.h>
#include <memory>
#include <list>
#include <mutex>
#include <algorithm>

using namespace std;

class dimensions
{ // ALONB -make this class a bit smarter, getters, setters, etc, check init, etc..
public:
    static sf::Vector2u screenDimentions;
    static sf::Vector2u activeGameDimentions;
    static uint32_t activeGameYOffset;
};
// Definition of static members outside the class

class RegularSprite
{
public:
    RegularSprite(string filePath, float scale) // ALONB - game screen dimensions should be retrieved every time or just saved? maybe it's a problem because the speed is also based on the screen dimentions and is set far above...
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

        float desiredWidth = dimensions::activeGameDimentions.x * scale; // Example desired width in pixels
        float imageScale = desiredWidth / originalSize.x;

        // sprite.setPosition(500, 500);
        // Create a sprite from the texture
        // cout << "imageScale " << imageScale << endl;
        sprite.setTexture(texture);
        //  sprite.setPosition(locationPix.x, locationPix.y);
        sprite.setScale(imageScale, imageScale);
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

union pointerToSprintsUnion
{
    sf::Drawable *drawable;
    list<unique_ptr<sf::Drawable>> *listOfDrawables;
};

class GameShapes
{
private:
    static unique_ptr<GameShapes> instance;

    uint32_t blackout;

    unique_ptr<Player> player;
    list<unique_ptr<Shark>> sharks;
    list<unique_ptr<Meduz>> meduzes;
    list<unique_ptr<Bubble>> bubbles;
    list<unique_ptr<sf::RectangleShape>> numCountdown;

    // vector<bubbles> score;

    // All Items to draw
    // bool is - isLlist
    vector<sf::Drawable *> drawablesList;

    pair<bool, bool> isCollisions;

    void clearAll();

    void createObsticle(uint32_t xPos, uint32_t boardHeight, uint32_t wallThickness, uint32_t spacing);
    // unique_ptr<sf::Shape> shapeFactory();

    float clamp(float value, float minVal, float maxVal) // Just in case CPP is older than 17 used explicit function.
    {
        return max(minVal, min(maxVal, value));
    }

public:
    // GameShapes(uint32_t x, uint32_t y);
    GameShapes() { blackout = false; }
    GameShapes operator=(GameShapes &) = delete;
    GameShapes(GameShapes &) = delete;
    static GameShapes *getGameShapes();

    std::mutex _mutex; // Define the mutex

    // void clearAll(); // ALONB - make all these list part of a vector for safer cleaning.
    void setActiveGame();

    void setCountDown(uint8_t num);

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

    void createNewShark();
    void createNewMeduz();
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
    void flickerScreen();
    // first is obstible and second is life
};

// ALONB make see with waves on top