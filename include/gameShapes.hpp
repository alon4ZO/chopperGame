#pragma once
#include <SFML/Graphics.hpp>
#include <definitions.h>
#include <memory>
#include <list>
#include <mutex>
#include <algorithm>

using namespace std;

class GameShapes
{
private:
    static unique_ptr<GameShapes> instance;

    uint32_t gameScreenWidth;
    uint32_t gameScreenHeight;
    uint32_t wallThickness;
    list<unique_ptr<sf::Shape>> walls;
    list<unique_ptr<sf::Shape>> chopper;
    list<unique_ptr<sf::Shape>> obsticals;
    list<unique_ptr<sf::Shape>> countDown;

    void createObsticle(uint32_t xPos, uint32_t boardHeight, uint32_t wallThickness, uint32_t spacing);
    unique_ptr<sf::Shape> shapeFactory();

    float clamp(float value, float minVal, float maxVal) // Just in case CPP is older than 17 used explicit function.
    {
        return max(minVal, min(maxVal, value));
    }

public:
    // GameShapes(uint32_t x, uint32_t y);
    GameShapes() = default;
    GameShapes operator=(GameShapes &) = delete;
    GameShapes(GameShapes &) = delete;
    static GameShapes *getGameShapes();

    std::mutex _mutex; // Define the mutex

    void clearAll(); // ALONB - make all these list part of a vector for safer cleaning.
    void setActiveGame();

    void setCountDown(uint8_t num);

    void setGameScreenDimensions(uint32_t gameScreenWidth, uint32_t gameScreenHeight)
    {
        this->gameScreenWidth = gameScreenWidth;
        this->gameScreenHeight = gameScreenHeight;
    }

    const list<unique_ptr<sf::Shape>> &getWalls() const { return walls; }
    const list<unique_ptr<sf::Shape>> &getchopper() const { return chopper; }
    const list<unique_ptr<sf::Shape>> &getobsticals() const { return obsticals; }
    const list<unique_ptr<sf::Shape>> &getCountDown() const { return countDown; }
    void updateObsicles(float dt);

    void createNewObstible();
    void cleanUpOldObsticles();
    void moveChopper(pair<float, float> direction) // ALONB - move this from here.
    {
        for (auto &i : chopper)
        {
            cout << direction.first << " " << direction.second << endl;

            i->move(direction.first, 0.0f);
            i->move(0.0f, direction.second);

            auto bounds = i->getGlobalBounds();
            sf::Vector2f position;
            // position.x = clamp(bounds.left + direction.first, 0, gameScreenWidth - bounds.width) + 100;
            // position.y = clamp(bounds.top + direction.second, wallThickness, gameScreenWidth - bounds.height);

            position.x = clamp(bounds.left, 0, gameScreenWidth - bounds.width);
            position.y = clamp(bounds.top, wallThickness, gameScreenHeight - bounds.height);
            // position.y = clamp(bounds.top + direction.second, wallThickness, gameScreenWidth - bounds.height);

            i->setPosition(position);

            // if (position.x < 0)
            // {
            //     position.x = 0;
            // }
            // else if (position.x > gameScreenWidth - i->getSize().x)
            // {
            //     position.x = gameScreenWidth - i->getSize().x;
            // }

            // if (position.y < wallThickness)
            // {
            //     position.y = wallThickness;
            // }
            // else if (position.y > gameScreenHeight - i->getSize().y)
            // {
            //     position.y = gameScreenHeight - i->getSize().y;
            // }

            // i->setPosition(position);

            // sf::Vector2f newPosition
            // if position.x <
        }
    }
};

// ALONB make see with waves on top