#pragma once
#include <SFML/Graphics.hpp>
#include <definitions.h>
#include <memory>
#include <vector>

using namespace std;

class GameShapes
{
private:
    uint32_t boardWidth;
    uint32_t boardHeight;
    uint32_t wallThickness;
    vector<unique_ptr<sf::Shape>> walls;
    vector<unique_ptr<sf::Shape>> chopper;
    vector<unique_ptr<sf::Shape>> obsticals;

    void createObsticle();
    unique_ptr<sf::Shape> shapeFactory();

public:
    GameShapes(uint32_t x, uint32_t y);
    const vector<unique_ptr<sf::Shape>> &getWalls() const { return walls; }
    const vector<unique_ptr<sf::Shape>> &getchopper() const { return chopper; }
    const vector<unique_ptr<sf::Shape>> &getobsticals() const { return obsticals; }
};
