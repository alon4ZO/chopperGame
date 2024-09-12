// ALONB - enable some warnings and errors in compiler
#include <SFML/Graphics.hpp>
#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <random>
#include <mutex>

using namespace std;

unique_ptr<GameShapes> GameShapes::instance = nullptr;

class shapeFactory // ALONB - put this below
{
private:
    static uint32_t numHeightPixels;
    static uint32_t numWidthPixels;
    static uint32_t getRandomNumber(uint32_t min, uint32_t max) // ALONB - how could this class own some of the dimentions?
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

    static unique_ptr<sf::Shape> createEmptyBlack(uint32_t xDim, uint32_t yDim, uint32_t wallThickness)
    {
        unique_ptr<sf::RectangleShape> shape = make_unique<sf::RectangleShape>();
        shape->setSize(sf::Vector2f(xDim, yDim));
        shape->setFillColor(sf::Color::Black);
        shape->setPosition(0, wallThickness);
        return shape;
    }

    static unique_ptr<sf::Shape> createObsticle(uint32_t xPos,
                                                uint32_t boardHeight,
                                                uint32_t wallThickness,
                                                uint32_t spacing) // ALONB - the spacing has to be re moved.
    {
        uint32_t xDim = getRandomNumber(spacing / 4, spacing * 12 / 10);
        uint32_t yDim = getRandomNumber((boardHeight - wallThickness) / 13, (boardHeight - wallThickness) * 1.2 / 13);
        uint32_t yPos = getRandomNumber(wallThickness, boardHeight - yDim);

        static sf::Texture texture;
        if (!texture.loadFromFile("C:\\ws\\chopperGame\\pic\\grassTexture.png"))
        {
            cout << "Fail" << endl; // Failed to load texture
        }

        texture.setRepeated(true);

        unique_ptr<sf::RectangleShape> obsticle;
        obsticle = make_unique<sf::RectangleShape>();
        obsticle->setSize(sf::Vector2f(xDim, yDim));
        obsticle->setFillColor(sf::Color(70, 107, 41));
        obsticle->setTexture(&texture);
        // obsticle->setTextureRect(sf::IntRect(0, 0, 300, 300)); // Size of the texture to repeat
        obsticle->setPosition(xPos, yPos);
        return obsticle;
    }

    static unique_ptr<sf::Shape> createChopper(uint32_t gameScreenWidth,
                                               uint32_t gameScreenHeight)
    {
        uint32_t xDim = gameScreenWidth / 20; // ALONB - change all these to X percentage, and add to definitions.
        uint32_t yDim = gameScreenHeight / 20;
        uint32_t yPos = (gameScreenHeight - yDim) / 2;
        uint32_t xPos = gameScreenWidth / 50;

        unique_ptr<sf::RectangleShape> mainBody;
        mainBody = make_unique<sf::RectangleShape>();
        mainBody->setSize(sf::Vector2f(xDim, yDim));
        mainBody->setFillColor(sf::Color::Blue);
        mainBody->setPosition(xPos, yPos);
        return mainBody;
    }

    static list<unique_ptr<sf::Shape>> createNum1(uint32_t boardWidth, uint32_t boardHeight)
    {
        list<unique_ptr<sf::Shape>> shapes;

        uint32_t xDim, yDim, xPos, yPos;

        xDim = boardWidth / 30;
        yDim = boardHeight / NUMBER_HEIGHT_RATIO;
        xPos = boardWidth / 2;
        yPos = (boardHeight - yDim) / 2;

        unique_ptr<sf::RectangleShape> one_1;
        one_1 = make_unique<sf::RectangleShape>();
        one_1->setSize(sf::Vector2f(xDim, yDim));
        one_1->setFillColor(sf::Color::Yellow);
        one_1->setPosition(xPos, yPos);

        shapes.push_back(move(one_1));
        return shapes;
    }

    static list<unique_ptr<sf::Shape>> createNum2(uint32_t boardWidth, uint32_t boardHeight)
    {
        list<unique_ptr<sf::Shape>> shapes;

        uint32_t xDim, yDim, xPos, yPos;

        xDim = boardWidth / 8;
        yDim = boardHeight / (NUMBER_HEIGHT_RATIO * 5);
        xPos = (boardWidth - xDim) / 2;
        yPos = (boardHeight - yDim) / 2;

        unique_ptr<sf::RectangleShape> two_1;
        two_1 = make_unique<sf::RectangleShape>();
        two_1->setSize(sf::Vector2f(xDim, yDim));
        two_1->setFillColor(sf::Color::Yellow);
        two_1->setPosition(xPos, yPos);

        unique_ptr<sf::RectangleShape> two_2;
        two_2 = make_unique<sf::RectangleShape>();
        two_2->setSize(sf::Vector2f(xDim, yDim));
        two_2->setFillColor(sf::Color::Yellow);
        two_2->setPosition(xPos, (boardHeight - boardHeight / NUMBER_HEIGHT_RATIO) / 2);

        unique_ptr<sf::RectangleShape> two_3;
        two_3 = make_unique<sf::RectangleShape>();
        two_3->setSize(sf::Vector2f(xDim, yDim));
        two_3->setFillColor(sf::Color::Yellow);
        two_3->setPosition(xPos, (boardHeight + boardHeight / NUMBER_HEIGHT_RATIO) / 2 - yDim);

        yDim = boardHeight / (NUMBER_HEIGHT_RATIO * 2);
        xDim = boardWidth / 30;
        xPos = (boardWidth + (boardWidth / 8)) / 2 - xDim;
        yPos = (boardHeight - (boardHeight / NUMBER_HEIGHT_RATIO)) / 2;

        unique_ptr<sf::RectangleShape> two_4;
        two_4 = make_unique<sf::RectangleShape>();
        two_4->setSize(sf::Vector2f(xDim, yDim));
        two_4->setFillColor(sf::Color::Yellow);
        two_4->setPosition(xPos, yPos);

        yDim = boardHeight / (NUMBER_HEIGHT_RATIO * 2);
        xDim = boardWidth / 30;
        xPos = (boardWidth - (boardWidth / 8)) / 2;
        yPos = (boardHeight) / 2;

        unique_ptr<sf::RectangleShape> two_5;
        two_5 = make_unique<sf::RectangleShape>();
        two_5->setSize(sf::Vector2f(xDim, yDim));
        two_5->setFillColor(sf::Color::Yellow);
        two_5->setPosition(xPos, yPos);

        shapes.push_back(move(two_1));
        shapes.push_back(move(two_2));
        shapes.push_back(move(two_3));
        shapes.push_back(move(two_4));
        shapes.push_back(move(two_5));
        return shapes;
    }
    static list<unique_ptr<sf::Shape>> createNum3(uint32_t boardWidth, uint32_t boardHeight)
    {
        list<unique_ptr<sf::Shape>> shapes;

        uint32_t xDim, yDim, xPos, yPos;

        xDim = boardWidth / 8;
        yDim = boardHeight / (NUMBER_HEIGHT_RATIO * 5);
        xPos = (boardWidth - xDim) / 2;
        yPos = (boardHeight - yDim) / 2;

        unique_ptr<sf::RectangleShape> three_1;
        three_1 = make_unique<sf::RectangleShape>();
        three_1->setSize(sf::Vector2f(xDim, yDim));
        three_1->setFillColor(sf::Color::Yellow);
        three_1->setPosition(xPos, yPos);

        unique_ptr<sf::RectangleShape> three_2;
        three_2 = make_unique<sf::RectangleShape>();
        three_2->setSize(sf::Vector2f(xDim, yDim));
        three_2->setFillColor(sf::Color::Yellow);
        three_2->setPosition(xPos, (boardHeight - boardHeight / NUMBER_HEIGHT_RATIO) / 2);

        unique_ptr<sf::RectangleShape> three_3;
        three_3 = make_unique<sf::RectangleShape>();
        three_3->setSize(sf::Vector2f(xDim, yDim));
        three_3->setFillColor(sf::Color::Yellow);
        three_3->setPosition(xPos, (boardHeight + boardHeight / NUMBER_HEIGHT_RATIO) / 2 - yDim);

        yDim = boardHeight / NUMBER_HEIGHT_RATIO;
        xDim = boardWidth / 30;
        xPos = (boardWidth + (boardWidth / 8)) / 2 - xDim;
        yPos = (boardHeight - yDim) / 2;

        unique_ptr<sf::RectangleShape> three_4;
        three_4 = make_unique<sf::RectangleShape>();
        three_4->setSize(sf::Vector2f(xDim, yDim));
        three_4->setFillColor(sf::Color::Yellow);
        three_4->setPosition(xPos, yPos);

        shapes.push_back(move(three_1));
        shapes.push_back(move(three_2));
        shapes.push_back(move(three_3));
        shapes.push_back(move(three_4));
        return shapes;
    };
};

GameShapes *GameShapes::getGameShapes()
{
    if (instance == nullptr)
    {
        instance = make_unique<GameShapes>();
        cout << "[GameShapes] - Creating game shapes" << endl;
    }
    return instance.get();
}

void GameShapes::setActiveGame() // This should be all the time... ALONB
{
    cout << "[GameShapes] - GameShapes - Creating walls, chopper" << endl;
    wallThickness = gameScreenHeight / GAME_BOARD_WALL_WIDTH_RATIO;

    unique_ptr<sf::Shape> ceiling = shapeFactory::createBorder(gameScreenWidth, wallThickness, 0);
    unique_ptr<sf::Shape> chopperBody = shapeFactory::createChopper(gameScreenWidth, gameScreenHeight);

    chopper.push_back(move(chopperBody));
    walls.push_back(move(ceiling));
    isCollisions = {false, false};
};

void GameShapes::setCountDown(uint8_t num)
{
    list<unique_ptr<sf::Shape>> numShapes;
    switch (num)
    {
    case 1:
        numShapes = shapeFactory::createNum1(gameScreenWidth, gameScreenHeight);
        break;
    case 2:
        numShapes = shapeFactory::createNum2(gameScreenWidth, gameScreenHeight);
        break;
    case 3:
        numShapes = shapeFactory::createNum3(gameScreenWidth, gameScreenHeight);
        break;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    countDown = move(numShapes);
}

void GameShapes::updateObsicles(float dt)
{

    // const float speed = 600.0f; // this was not bad
    // const int spacing = 100;

    const float speed = 200.0f; // pixels per second
    const int spacing = 100;

    // std::lock_guard<std::mutex> lock(_mutex);

    // Move all obsitcles to the left according to time passed and speed.
    for (auto &i : obsticals)
    {
        i->move(-speed * dt, 0.0f);
    }

    // create obsticle if screen is empty.
    // if (obsticals.size() == 0)
    // {
    //     unique_ptr<sf::Shape> obsticle = shapeFactory::createObsticle(gameScreenWidth, gameScreenHeight, wallThickness, spacing);
    //     obsticals.push_back(move(obsticle));
    //     return;
    // }

    // // if last obsticle is advanced enough, introduce a new obsitcle.
    // auto &newestObsticle = obsticals.back();
    // sf::Vector2f positionOfNewestObsticle = newestObsticle->getPosition();

    // if (positionOfNewestObsticle.x < gameScreenWidth - spacing)
    // {
    //     unique_ptr<sf::Shape> obsticle = shapeFactory::createObsticle(gameScreenWidth, gameScreenHeight, wallThickness, spacing);
    //     obsticals.push_back(move(obsticle));
    //     return;
    // }

    // remove old obsticle if necessary
    // auto &oldestObsticle = obsticals.front();
    // sf::Rect<float> floatRect = oldestObsticle->getGlobalBounds();

    // if (floatRect.left + floatRect.width < 0)
    // {
    //     obsticals.pop_front();
    // }

    // cout << obsticals.size() << endl;
    // check if to remove first object
};

void GameShapes::clearAll()
{
    std::lock_guard<std::mutex> lock(_mutex);

    // walls.clear(); // ALONB - make all these list part of a vector for safer cleaning.
    chopper.clear();
    obsticals.clear();
    countDown.clear();
}

void GameShapes::createNewObstible()
{

    unique_ptr<sf::Shape> obsticle = shapeFactory::createObsticle(gameScreenWidth, gameScreenHeight, wallThickness, 200);
    std::lock_guard<std::mutex> lock(_mutex);
    obsticals.push_back(move(obsticle));
}

void GameShapes::cleanUpOldObsticles()
{
    std::lock_guard<std::mutex> lock(_mutex);

    while (obsticals.size() > 0)
    {

        auto &oldestObsticle = obsticals.front();
        sf::Rect<float> floatRect = oldestObsticle->getGlobalBounds();

        if (floatRect.left + floatRect.width > 0)
        {
            break;
        }

        obsticals.pop_front();
    }
}

void GameShapes::checkCollisions()
{

    for (auto &i : obsticals)
    {
        if (chopper.front()->getGlobalBounds().intersects(i->getGlobalBounds()))
        {
            isCollisions.first = true;
            return; // ALONB - add extra life.
        }
    }
}

void GameShapes::flickerScreen()
{
    if (blackout.size() == 1)
    {
        blackout.pop_back();
    }
    else
    {

        unique_ptr<sf::Shape> shape = shapeFactory::createEmptyBlack(gameScreenWidth, gameScreenHeight, wallThickness); // ALONB - ad this level, they should all be called shape or something generic.
        std::lock_guard<std::mutex> lock(_mutex);
        blackout.push_back(move(shape));
    }
}
