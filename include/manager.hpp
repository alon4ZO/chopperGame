#pragma once
#include <SFML/Graphics.hpp>
#include <definitions.h>
#include <memory>
#include <list>

using namespace std;

class GameShapes
{
private:
    uint32_t boardWidth;
    uint32_t boardHeight;
    uint32_t wallThickness;
    list<unique_ptr<sf::Shape>> walls;
    list<unique_ptr<sf::Shape>> chopper;
    list<unique_ptr<sf::Shape>> obsticals;

    void createObsticle(uint32_t xPos, uint32_t boardHeight, uint32_t wallThickness, uint32_t spacing);
    unique_ptr<sf::Shape> shapeFactory();

public:
    GameShapes(uint32_t x, uint32_t y);
    const list<unique_ptr<sf::Shape>> &getWalls() const { return walls; }
    const list<unique_ptr<sf::Shape>> &getchopper() const { return chopper; }
    const list<unique_ptr<sf::Shape>> &getobsticals() const { return obsticals; }
    void updateObsicles(float dt);
};
