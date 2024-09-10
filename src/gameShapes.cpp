#include <SFML/Graphics.hpp>
#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <random>

using namespace std;

class shapeFactory
{
private:
    static uint32_t getRandomNumber(uint32_t min, uint32_t max)
    {
        // Create a random number generator and seed it with a random device
        std::random_device rd;
        std::mt19937 eng(rd());

        // Create a uniform distribution in the specified range
        std::uniform_int_distribution<> distr(min, max);

        // Return a random number
        return distr(eng);
    }

public:
    static unique_ptr<sf::Shape> createBorder(uint32_t xDim, uint32_t yDim, uint32_t yCord)
    {
        unique_ptr<sf::RectangleShape> border = make_unique<sf::RectangleShape>();
        border->setSize(sf::Vector2f(xDim, yDim));
        border->setFillColor(sf::Color GAME_BOARD_WALL_COLORS);

        border->setPosition(0, yCord);
        return border;
    }

    static unique_ptr<sf::Shape> createObsticle(uint32_t xPos,
                                                uint32_t boardHeight,
                                                uint32_t wallThickness,
                                                uint32_t spacing)
    {
        uint32_t xDim = getRandomNumber(spacing / 4, spacing * 8 / 4);
        uint32_t yDim = getRandomNumber((boardHeight - 2 * wallThickness) / 15, (boardHeight - 2 * wallThickness) * 2 / 15);
        uint32_t yPos = getRandomNumber(wallThickness, boardHeight - (wallThickness + yDim));
        ;

        unique_ptr<sf::RectangleShape> obsticle;
        obsticle = make_unique<sf::RectangleShape>();
        obsticle->setSize(sf::Vector2f(xDim, yDim));
        obsticle->setFillColor(sf::Color::Red);
        obsticle->setPosition(xPos, yPos);
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

    unique_ptr<sf::Shape> ceiling = shapeFactory::createBorder(boardWidth, wallThickness, 0);
    unique_ptr<sf::Shape> floor = shapeFactory::createBorder(boardWidth, wallThickness, boardHeight - wallThickness);

    walls.push_back(move(floor));
    walls.push_back(move(ceiling));

    cout << "[GameShapes] Creating first shape" << endl;
};

void GameShapes::updateObsicles(float dt)
{

    const float speed = 300.0f; // pixels per second
    const int spacing = 200;

    // Move all obsitcles to the left according to time passed and speed.
    for (auto &i : obsticals)
    {
        i->move(-speed * dt, 0.0f);
    }

    // create obsticle if screen is empty.
    if (obsticals.size() == 0)
    {
        unique_ptr<sf::Shape> obsticle = shapeFactory::createObsticle(boardWidth, boardHeight, wallThickness, spacing);
        obsticals.push_back(move(obsticle));
        return;
    }

    // // if last obsticle is advanced enough, introduce a new obsitcle.
    auto &newestObsticle = obsticals.back();
    sf::Vector2f positionOfNewestObsticle = newestObsticle->getPosition();

    if (positionOfNewestObsticle.x < boardWidth - spacing)
    {
        unique_ptr<sf::Shape> obsticle = shapeFactory::createObsticle(boardWidth, boardHeight, wallThickness, spacing);
        obsticals.push_back(move(obsticle));
        return;
    }

    // remove old obsticle if necessary
    auto &oldestObsticle = obsticals.front();
    sf::Rect<float> floatRect = oldestObsticle->getGlobalBounds();

    if (floatRect.left + floatRect.width < 0)
    {
        obsticals.pop_front();
    }

    // cout << obsticals.size() << endl;
    // check if to remove first object
};
