// ALONB - enable some warnings and errors in compiler
#include <SFML/Graphics.hpp>
#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <mutex>
#include <definitions.h>
#include <memory>
#include <list>

using namespace std;

Meduz::Meduz(float scale, float verticleSpeed) : Obsticle(getPathForAsset("meduz", ".png", 2), scale, {0, verticleSpeed})
{
    setLocation(getRandomNumber(static_cast<float>(0), dimensions::activeGameDimentions.x - getBounds().width),
                dimensions::screenDimentions.y);
};
Shark::Shark(float scale, float horizontalSpeed) : Obsticle(getPathForAsset("shark", ".png"), scale, {horizontalSpeed, 0})
{
    setLocation(dimensions::activeGameDimentions.x,
                dimensions::activeGameYOffset + (getRandomNumber(static_cast<float>(0), dimensions::activeGameDimentions.y - getBounds().height)));
};
Bubble::Bubble(sf::FloatRect playerBoundsRect) : MovingSprite(getPathForAsset("bubble", ".png"), GAME_BOARD_BUBBLE_X_SCALE, {0, GAME_BOARD_BUBBLE_VERTICAL_SPEED})
{
    setLocation(playerBoundsRect.left + playerBoundsRect.width * 0.2f, playerBoundsRect.top + playerBoundsRect.height * 0.5);
};

Prize::Prize() : RegularSprite(getPathForAsset("prize", ".png"), 0.05)
{
    // make sure the location supports the boyancy. ALONB - also, spelling boyancy?
    setLocation(getRandomNumber(0u, static_cast<uint32_t>(dimensions::activeGameDimentions.x - getBounds().width)),
                getRandomNumber(dimensions::activeGameYOffset, static_cast<uint32_t>(dimensions::activeGameDimentions.y - getBounds().height)));

    yMid = getBounds().top;
    yDelta = 0.025;
    timeBeforeFadingSec = 1;
    fadeTimeInSec = 5;
    fading = false;
}

Player::Player() : MovingSprite(getPathForAsset("player", ".png"), GAME_BOARD_PLAYER_X_SIZE_RATIO, {GAME_BOARD_PLAYER_SPEED_X_SCREENS_PER_SEC, GAME_BOARD_PLAYER_SPEED_Y_SCREENS_PER_SEC}) // ALONB - change these pixels per sec
{
    currentXSpeed = 0;
    currentYSpeed = 0;
    accelarationX = GAME_BOARD_PLAYER_SPEED_X_SCREENS_PER_SEC * GAME_BOARD_PLAYER_ACCELERATION_FACTOR;
    accelarationY = GAME_BOARD_PLAYER_SPEED_Y_SCREENS_PER_SEC * GAME_BOARD_PLAYER_ACCELERATION_FACTOR;
    setLocation(dimensions::activeGameDimentions.x * GAME_BOARD_PLAYER_X_OFFSET_RATIO,
                dimensions::activeGameYOffset + (dimensions::activeGameDimentions.y - getBounds().height) / 2);
};

ExtraLifeIcon::ExtraLifeIcon() : MovingSprite(getPathForAsset("extraLife", ".png"), GAME_BOARD_EXTRA_LIFE_ICON_X_RATIO, {0, GAME_BOARD_EXTRA_LIFE_ICON_SPEED_Y_SCREENS_PER_SEC})

{
    setLocation(dimensions::activeGameDimentions.x - getBounds().width * 2, // ALONB - this 2 is a def.
                dimensions::activeGameYOffset + (dimensions::activeGameDimentions.y / 2));

    fadeTimeConst = 2;
    fadeTimeInSec = fadeTimeConst;
};

// Factories:

list<unique_ptr<sf::RectangleShape>> simpleObjectFactory::createNum1()
{

    uint32_t boardWidth = dimensions::screenDimentions.x;
    uint32_t boardHeight = dimensions::screenDimentions.y;
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
list<unique_ptr<sf::RectangleShape>> simpleObjectFactory::createNum2()
{

    uint32_t boardWidth = dimensions::screenDimentions.x;
    uint32_t boardHeight = dimensions::screenDimentions.y;
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
list<unique_ptr<sf::RectangleShape>> simpleObjectFactory::createNum3()
{

    uint32_t boardWidth = dimensions::screenDimentions.x;
    uint32_t boardHeight = dimensions::screenDimentions.y;
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
