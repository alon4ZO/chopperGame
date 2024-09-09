#include <SFML/Graphics.hpp>
#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>

using namespace std;

class shapeFactory
{
public:
    static unique_ptr<sf::Shape> createBorder(uint32_t xDim, uint32_t yDim, uint32_t yCord)
    {
        unique_ptr<sf::RectangleShape> border = make_unique<sf::RectangleShape>();
        border->setSize(sf::Vector2f(xDim, yDim));
        border->setFillColor(sf::Color GAME_BOARD_WALL_COLORS);

        border->setPosition(0, yCord);
        return border;
    }

    static unique_ptr<sf::Shape> createObsticle()
    {
        unique_ptr<sf::RectangleShape> obsticle = make_unique<sf::RectangleShape>();
        obsticle->setSize(sf::Vector2f(10, 10));
        obsticle->setFillColor(sf::Color::Red);
        obsticle->setPosition(70, 70);
        return obsticle;
    }

    // static unique_ptr<sf::Shape> createChopper()
    // {
    // }
};

GameShapes::GameShapes(uint32_t x, uint32_t y) : boardWidth(x), boardHeight(y)
{
    cout << "[GameShapes] - GameShapes - Creating walls, chopper" << endl;
    wallThickness = boardHeight / GAME_BOARD_WALL_WIDTH_RATIO;

    // walls.push_back(move(floor));
    unique_ptr<sf::Shape> ceiling = shapeFactory::createBorder(boardWidth, wallThickness, 0);
    unique_ptr<sf::Shape> floor = shapeFactory::createBorder(boardWidth, wallThickness, boardHeight - wallThickness);
    unique_ptr<sf::Shape> obsticle = shapeFactory::createObsticle();

    walls.push_back(move(floor));
    walls.push_back(move(ceiling));
    obsticals.push_back(move(obsticle));

    cout << "[GameShapes] Creating first shape" << endl;
};
