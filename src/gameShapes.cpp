// ALONB - enable some warnings and errors in compiler
#include <SFML/Graphics.hpp>
#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <mutex>
#include <filesystem>
#include <definitions.h>

using namespace std;

unique_ptr<GameShapes> GameShapes::instance = nullptr;

// class dimensions
// { // ALONB -make this class a bit smarter, getters, setters, etc, check init, etc..
// public:
//     static sf::Vector2u screenDimentions;
//     static sf::Vector2u activeGameDimentions;
//     static uint32_t activeGameYOffset;
// };
// Definition of static members outside the class
sf::Vector2u dimensions::screenDimentions;     // Definition
sf::Vector2u dimensions::activeGameDimentions; // Definition
uint32_t dimensions::activeGameYOffset;        // Definition (initialize if necessary)

class shapeFactory // ALONB - put this below
{
private:
    static uint32_t numHeightPixels;
    static uint32_t numWidthPixels;

public:
    static unique_ptr<sf::Shape> createBorder(uint32_t xDim, uint32_t yDim, uint32_t yCord)
    {
        unique_ptr<sf::RectangleShape> border = make_unique<sf::RectangleShape>();
        border->setSize(sf::Vector2f(xDim, yDim));
        // border->setFillColor(sf::Color GAME_BOARD_WALL_COLORS);

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
        // uint32_t xDim = getRandomNumber(spacing / 4, spacing * 12 / 10);
        // uint32_t yDim = getRandomNumber((boardHeight - wallThickness) / 13, (boardHeight - wallThickness) * 1.2 / 13);
        // uint32_t yPos = getRandomNumber(wallThickness, boardHeight - yDim);

        // static sf::Texture texture;
        // if (!texture.loadFromFile("C:\\ws\\chopperGame\\pic\\grassTexture.png"))
        // {
        //     cout << "Fail" << endl; // Failed to load texture
        // }

        // texture.setRepeated(true);

        unique_ptr<sf::RectangleShape> obsticle;
        // obsticle = make_unique<sf::RectangleShape>();
        // obsticle->setSize(sf::Vector2f(xDim, yDim));
        // obsticle->setFillColor(sf::Color(70, 107, 41));
        // obsticle->setTexture(&texture);
        // // obsticle->setTextureRect(sf::IntRect(0, 0, 300, 300)); // Size of the texture to repeat
        // obsticle->setPosition(xPos, yPos);
        return obsticle;
    }

    // static unique_ptr<sf::Shape> createPlayer(uint32_t screenDimentions.x,
    //                                           uint32_t screenDimentions.y)
    // {
    //     uint32_t xDim = screenDimentions.x / 20; // ALONB - change all these to X percentage, and add to definitions.
    //     uint32_t yDim = screenDimentions.y / 20;
    //     uint32_t yPos = (screenDimentions.y - yDim) / 2;
    //     uint32_t xPos = screenDimentions.x / 50;

    //     unique_ptr<sf::RectangleShape> mainBody;
    //     mainBody = make_unique<sf::RectangleShape>();
    //     mainBody->setSize(sf::Vector2f(xDim, yDim));
    //     mainBody->setFillColor(sf::Color::Blue);
    //     mainBody->setPosition(xPos, yPos);
    //     return mainBody;
    // }

    static list<unique_ptr<sf::RectangleShape>> createNum1(uint32_t boardWidth, uint32_t boardHeight)
    {
        list<unique_ptr<sf::RectangleShape>> shapes;

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

    static string getPathForPng(string fileName, uint8_t randomOptions = 1) // Put this with the random num generator in utils tab.
    {

        const char *currentFilePath = __FILE__;
        uint8_t option;
        string flavor = "";

        // std::cout << "Current File Path: " << currentFilePath << std::endl;

        // Using filesystem to manipulate paths (C++17 and later)
        std::filesystem::path path(currentFilePath);

        // Getting the parent directory
        std::filesystem::path parentDirectory = path.parent_path().parent_path();

        if (randomOptions != 1)
        {
            option = getRandomNumber((0), randomOptions - 1);
            flavor = "_" + to_string(option);
        }

        // Construct a relative path from the current file location
        // std::filesystem::path relativePath = parentDirectory / "pic" / "player.png"; // Example relative
        std::filesystem::path relativePath = parentDirectory / "pic" / (fileName + flavor + ".png"); // Example relative

        // Outputting the resolved relative path

        string res = relativePath.u8string();

        for (char &c : res)
        {
            if (c == '\\')
            {
                c = '/';
            }
        }
        // std::cout << "Relative Path: " << res << std::endl;

        // switch
        //     list<unique_ptr<sf::RectangleShape>> shapes;

        // uint32_t xDim, yDim, xPos, yPos;

        // xDim = boardWidth / 30;
        // yDim = boardHeight / NUMBER_HEIGHT_RATIO;
        // xPos = boardWidth / 2;
        // yPos = (boardHeight - yDim) / 2;

        // unique_ptr<sf::RectangleShape> one_1;
        // one_1 = make_unique<sf::RectangleShape>();
        // one_1->setSize(sf::Vector2f(xDim, yDim));
        // one_1->setFillColor(sf::Color::Yellow);
        // one_1->setPosition(xPos, yPos);

        // shapes.push_back(move(one_1));
        // return shapes;
        return res;
    }

    static list<unique_ptr<sf::RectangleShape>> createNum2(uint32_t boardWidth, uint32_t boardHeight)
    {
        list<unique_ptr<sf::RectangleShape>> shapes;

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
    static list<unique_ptr<sf::RectangleShape>> createNum3(uint32_t boardWidth, uint32_t boardHeight) // ALONB - this should be "drawable lowercase"
    {
        list<unique_ptr<sf::RectangleShape>> shapes;

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
    }
    return instance.get();
}

void GameShapes::clearAll()
{

    //
    // for ()
    sharks.clear();
    meduzes.clear();
    numCountdown.clear();
    bubbles.clear();
    player.release();
    // /itemsToDraw.clear();

    // rest player position ALONB
}

void GameShapes::setActiveGame()
{
    cout << "[GameShapes] - setActiveGame" << endl;

    isCollisions = {false, false};

    std::lock_guard<std::mutex> lock(_mutex);
    itemsToDraw.clear();

    player = make_unique<Player>(0.08);
};

vector<sf::Drawable *> &GameShapes::updateAndGetItemsToDraw()
{
    // cout << "[screen] - update" << endl;

    itemsToDraw.clear();
    if (player)
    {
        // cout << "T" << endl;
        itemsToDraw.push_back(player->getDrawable());
    }

    for (const auto &i : numCountdown)
    {
        // itemsToDraw.push_back(static_cast<sf::Drawable *>(i.get()));
        itemsToDraw.push_back(i.get()); // ALONB - use a shared pointer instead?
    }

    for (const auto &i : sharks)
    {
        // itemsToDraw.push_back(static_cast<sf::Drawable *>(i.get()));
        itemsToDraw.push_back(i->getDrawable()); // ALONB - use a shared pointer instead?
    }

    for (const auto &i : meduzes)
    {
        // itemsToDraw.push_back(static_cast<sf::Drawable *>(i.get()));
        itemsToDraw.push_back(i->getDrawable()); // ALONB - use a shared pointer instead?
    }

    return itemsToDraw;
}

void GameShapes::setCountDown(uint8_t num)
{
    list<unique_ptr<sf::RectangleShape>> numShapes;
    switch (num)
    {
    case 1:
        numShapes = shapeFactory::createNum1(dimensions::screenDimentions.x, dimensions::screenDimentions.y);
        break;
    case 2:
        numShapes = shapeFactory::createNum2(dimensions::screenDimentions.x, dimensions::screenDimentions.y);
        break;
    case 3:
        numShapes = shapeFactory::createNum3(dimensions::screenDimentions.x, dimensions::screenDimentions.y);
        break;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    numCountdown = move(numShapes);
}

void GameShapes::updateMovables(float dt, pair<int8_t, int8_t> playerSteps) // ALONB add inputs here.
{

    // const float speed = 600.0f; // this was not bad
    // const int spacing = 100;

    // const float speed = 200.0f; // pixels per second
    // const int spacing = 100;

    // std::lock_guard<std::mutex> lock(_mutex);

    // Move all obsitcles to the left according to time passed and speed.

    for (auto &i : sharks)
    {
        i->advance(dt);
    }
    for (auto &i : meduzes)
    {
        i->advance(dt);
    }

    // for (auto &i : meduzes)
    // {
    //     i->advance(-speed * dt, 0.0f);
    // }

    // for (auto &i : bubbles)
    // {
    //     i->advance(-speed * dt, 0.0f);
    // }

    // check if player speed needs to change.

    // cout << static_cast<int32_t>(playerSteps.first) << " " << static_cast<int32_t>(playerSteps.second) << endl;
    // cout << "dt" << dt << endl;
    if (player)
    {
        player->advance(dt, playerSteps.first, playerSteps.second);
    }
    // create obsticle if screen is empty.
    // if (obsticals.size() == 0)
    // {
    //     unique_ptr<sf::Shape> obsticle = shapeFactory::createObsticle(screenDimentions.x, screenDimentions.y, wallThickness, spacing);
    //     obsticals.push_back(move(obsticle));
    //     return;
    // }

    // // if last obsticle is advanced enough, introduce a new obsitcle.
    // auto &newestObsticle = obsticals.back();
    // sf::Vector2f positionOfNewestObsticle = newestObsticle->getPosition();

    // if (positionOfNewestObsticle.x < screenDimentions.x - spacing)
    // {
    //     unique_ptr<sf::Shape> obsticle = shapeFactory::createObsticle(screenDimentions.x, screenDimentions.y, wallThickness, spacing);
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

void GameShapes::createNewShark()
{
    bool collisionDuringCreation;
    do
    {
        collisionDuringCreation = false;
        unique_ptr<Shark> newShark = make_unique<Shark>(0.1, -0.25);

        // MAKE SURE DOES NOT COLLIDE WITH OTHER SHARKS

        for (const auto &i : sharks)
        {
            // if (i->checkColision(newShark.get()->getBounds()))
            if (i->checkColision(newShark->getBounds()))
            {
                collisionDuringCreation = true;
                // cout << "COL" << endl;
                break;
            }
        }

        if (!collisionDuringCreation)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            sharks.push_back(move(newShark));
            // cout << "SOL" << endl;
        }

    } while (collisionDuringCreation);
}

void GameShapes::createNewMeduz() // ALONB the meduzes can collide?
{
    // ALONB - add a non colliding private function to be used by meduz, shark.
    bool collisionDuringCreation;
    do
    {
        collisionDuringCreation = false;
        unique_ptr<Meduz> newMeduz = make_unique<Meduz>(0.1, -0.25);

        // MAKE SURE DOES NOT COLLIDE WITH OTHER SHARKS

        for (const auto &i : meduzes)
        {
            // if (i->checkColision(newShark.get()->getBounds()))
            if (i->checkColision(newMeduz->getBounds()))
            {
                collisionDuringCreation = true;
                // cout << "COL" << endl;
                break;
            }
        }

        if (!collisionDuringCreation)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            meduzes.push_back(move(newMeduz));
            cout << "SOL" << endl;
        }

    } while (collisionDuringCreation);
}

void GameShapes::cleanUpOldObsticles()
{
    std::lock_guard<std::mutex> lock(_mutex);

    // use get Bounds insted of manual calc can use the check collision with the screen

    // while (obsticals.size() > 0)
    // {

    //     auto &oldestObsticle = obsticals.front();
    //     sf::Rect<float> floatRect = oldestObsticle->getGlobalBounds();

    //     if (floatRect.left + floatRect.width > 0)
    //     {
    //         break;
    //     }

    //     obsticals.pop_front();
    // }
}

void GameShapes::checkCollisions()
{

    for (auto &i : sharks)
    {
        if (i->checkColision(player->getBounds()))
        {
            isCollisions.first = true;
            break;
        }
    }

    for (auto &i : meduzes)
    {
        if (i->checkColision(player->getBounds()))
        {
            isCollisions.first = true;
            break;
        }
    }

    // for (auto &i : obsticals)
    // {
    //     if (chopper.front()->getGlobalBounds().intersects(i->getGlobalBounds()))
    //     {
    //         isCollisions.first = true;
    //         return; // ALONB - add extra life.
    //     }
    // }
}

void GameShapes::flickerScreen()
{
    blackout = blackout ? false : true;
    // if (blackout.size() == 1)
    // {
    //     blackout.pop_back();
    // }
    // else
    // {

    //     unique_ptr<sf::Shape> shape = shapeFactory::createEmptyBlack(screenDimentions.x, screenDimentions.y, wallThickness); // ALONB - ad this level, they should all be called shape or something generic.
    //     std::lock_guard<std::mutex> lock(_mutex);
    //     blackout.push_back(move(shape));
    // }
}

Meduz::Meduz(float scale, float verticleSpeed) : Obsticle(shapeFactory::getPathForPng("meduz", 2), scale, {0, verticleSpeed})
{
    setLocation(getRandomNumber(static_cast<float>(0), dimensions::activeGameDimentions.x - getBounds().width),
                dimensions::screenDimentions.y);
};
Shark::Shark(float scale, float horizontalSpeed) : Obsticle(shapeFactory::getPathForPng("shark"), scale, {horizontalSpeed, 0})
{
    setLocation(dimensions::activeGameDimentions.x,
                dimensions::activeGameYOffset + (getRandomNumber(static_cast<float>(0), dimensions::activeGameDimentions.y - getBounds().height)));
};
Bubble::Bubble(float scale, float verticleSpeed) : MovingSprite(shapeFactory::getPathForPng("bubble"), scale, {0, verticleSpeed}) {};
Player::Player(float scale) : MovingSprite(shapeFactory::getPathForPng("player"), scale, {GAME_BOARD_PLAYER_SPEED_X_SCREENS_PER_SEC, GAME_BOARD_PLAYER_SPEED_Y_SCREENS_PER_SEC}) // ALONB - change these pixels per sec
{
    setLocation(dimensions::activeGameDimentions.x * GAME_BOARD_PLAYER_X_OFFSET_RATIO,
                dimensions::activeGameYOffset + (dimensions::activeGameDimentions.y - getBounds().height) / 2);
};
