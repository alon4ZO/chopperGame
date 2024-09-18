// ALONB - enable some warnings and errors in compiler
#include <SFML/Graphics.hpp>
#include <iostream>
#include <screen.hpp>
// #include <gameShapes.hpp>
#include <mutex>
#include <filesystem>
#include <definitions.h>

using namespace std;

list<unique_ptr<sf::RectangleShape>> shapeFactory::createNum1(uint32_t boardWidth, uint32_t boardHeight)
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

string shapeFactory::getPathForPng(string fileName, uint8_t randomOptions) // Put this with the random num generator in utils tab.
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

    return res;
}

list<unique_ptr<sf::RectangleShape>> shapeFactory::createNum2(uint32_t boardWidth, uint32_t boardHeight)
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
list<unique_ptr<sf::RectangleShape>> shapeFactory::createNum3(uint32_t boardWidth, uint32_t boardHeight) // ALONB - this should be "drawable lowercase"
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