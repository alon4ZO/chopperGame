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

GeneralText::GeneralText(string displayString, float charSize, bool isBlink)
{
    this->isBlink = isBlink;
    string filePath = getPathForAsset("RowsOfSunflowers", ".ttf");
    if (!font.loadFromFile(filePath))
    {
        std::cerr << "Error loading font!" << std::endl;
        assert(0); // ALONB - use exception?
    }
    text.setString(displayString);
    text.setFont(font);
    text.setCharacterSize(charSize);
    text.setFillColor(sf::Color::Red);
}

ScoreText::ScoreText() : GeneralText("", dimensions::activeGameYOffset / 2)
{
    setPosition(dimensions::screenDimentions.x * 0.9, dimensions::activeGameYOffset * 0.2); // ALONB - CHANGE the positioning to be relative?
}

GameOverText::GameOverText() : GeneralText("Game Over", dimensions::screenDimentions.y / 4)
{
    setPosition((dimensions::screenDimentions.x - getBounds().width) / 2, (0));
}

string ScoresText::synthasizeText(uint32_t currentScore, uint32_t highScore)
{
    if (currentScore < highScore)
    {
        return "Good Try! high score is " + to_string(highScore);
    }

    else
    {
        return "High score reached - " + to_string(highScore) + "!";
    }
}

ScoresText::ScoresText(float yPosition, uint32_t currentScore, uint32_t highScore) : GeneralText(synthasizeText(currentScore, highScore), dimensions::screenDimentions.y / 6)
{
    setPosition((dimensions::screenDimentions.x - getBounds().width) / 2, yPosition);
}

pressEnterToRestart::pressEnterToRestart() : GeneralText("Press Enter To Play", dimensions::screenDimentions.y / 4, true) // ALONB - change name: add Text to pressEnterToRestart
{
    // setPosition((dimensions::screenDimentions.x - getBounds().width) / 2, dimensions::screenDimentions.y - getBounds().height);
    setPosition((dimensions::screenDimentions.x - getBounds().width) / 2, dimensions::screenDimentions.y - text.getCharacterSize() * 1.5);
}

// sprites:

RegularSprite::RegularSprite(string filePath, float scaleX, float scaleYoverride)
{
    if (!texture.loadFromFile(filePath))
    {
        std::cerr << "Error loading image!" << std::endl;
        assert(0); // ALONB - use exception?
    }
    sf::Vector2f originalSize(texture.getSize());

    // Calculate scale factors
    float imageScale;

    if (scaleYoverride == -1)
    {
        float desiredWidth = dimensions::screenDimentions.x * scaleX;
        imageScale = desiredWidth / originalSize.x;
    }
    else
    {
        float desiredHeight = dimensions::screenDimentions.y * scaleYoverride;
        imageScale = desiredHeight / originalSize.y;
    }

    // setAlpha(OBJECTS_MAX_COLOR_VALUE);
    sprite.setTexture(texture);
    sprite.setScale(imageScale, imageScale);
}

ScoresIcon::ScoresIcon() : RegularSprite(getPathForAsset("score_sign", ".png"), 0.03)
{
    setLocation(dimensions::activeGameDimentions.x / 2, (dimensions::activeGameYOffset * 1.1 - getBounds().height) / 2);
}

lifeIcon::lifeIcon(uint8_t id) : RegularSprite(getPathForAsset("player", ".png"), 0, GAME_SCREEN_WALL_WIDTH_RATIO * 0.3)
{
    setLocation(dimensions::screenDimentions.x * 0.03 + (getBounds().width + dimensions::screenDimentions.x * 0.03) * id,
                (dimensions::activeGameYOffset - getBounds().height) / 2);
}

ChangingSprite::ChangingSprite(string filePath, float scale, sf::Vector2f speedScreenPerSec, bool shouldFade, int32_t timeUntilFadeStartSec, int32_t fadeSpeedAlphaPerSec) : RegularSprite(filePath, scale),
                                                                                                                                                                             speedPixPerSecond({speedScreenPerSec.x * dimensions::activeGameDimentions.x, speedScreenPerSec.y * dimensions::activeGameDimentions.y}),
                                                                                                                                                                             timeUntilFadeStartSec(timeUntilFadeStartSec), fadingTimeSec(fadeSpeedAlphaPerSec),
                                                                                                                                                                             remainingFadeTimeSec(fadeSpeedAlphaPerSec),
                                                                                                                                                                             shouldFade(shouldFade) {};

void ChangingSprite::advance(float dt, int8_t x, int8_t y)
{
    sprite.move(dt * speedPixPerSecond.x * x, dt * speedPixPerSecond.y * y);

    // fade
    if (!shouldFade)
    {
        return;
    }

    timeUntilFadeStartSec -= dt;

    if (timeUntilFadeStartSec > 0)
    {
        return;
    }

    float alpha = (remainingFadeTimeSec / fadingTimeSec) * 255;
    setAlpha(alpha);
    remainingFadeTimeSec -= dt;

    if (remainingFadeTimeSec < 0)
    {
        sprite.setPosition(-1000, -1000);
    }
}

Meduz::Meduz(float scale, float verticleSpeed) : ChangingSprite(getPathForAsset("meduz", ".png", 2), scale, {0, verticleSpeed})
{
    setLocation(getRandomNumber(static_cast<float>(0), dimensions::activeGameDimentions.x - getBounds().width),
                dimensions::screenDimentions.y);
};

void Meduz::advance(float dt, int8_t x, int8_t y)
{
    float dy = dt * speedPixPerSecond.y;

    if ((this->getBounds().top + dy) < dimensions::activeGameYOffset)
    {
        speedPixPerSecond.y *= -1;
    }

    ChangingSprite::advance(dt, x, y);
}

ExtraLifeIcon::ExtraLifeIcon() : ChangingSprite(getPathForAsset("extraLife", ".png"), GAME_BOARD_EXTRA_LIFE_ICON_X_RATIO, {0, GAME_BOARD_EXTRA_LIFE_ICON_SPEED_Y_SCREENS_PER_SEC}, true, 0, 2)

{
    setLocation(dimensions::activeGameDimentions.x - getBounds().width * 2, // ALONB - this 2 is a def.
                dimensions::activeGameYOffset + (dimensions::activeGameDimentions.y / 2));

    // fadeTimeConst = 2;
    // fadeTimeInSec = fadeTimeConst;
};

Shark::Shark(float scale, float horizontalSpeed) : ChangingSprite(getPathForAsset("shark", ".png"), scale, {horizontalSpeed, 0})
{
    setLocation(dimensions::activeGameDimentions.x,
                dimensions::activeGameYOffset + (getRandomNumber(static_cast<float>(0), dimensions::activeGameDimentions.y - getBounds().height)));
};

Bubble::Bubble(sf::FloatRect playerBoundsRect) : ChangingSprite(getPathForAsset("bubble", ".png"), GAME_BOARD_BUBBLE_X_SCALE, {0, GAME_BOARD_BUBBLE_VERTICAL_SPEED})
{
    setLocation(playerBoundsRect.left + playerBoundsRect.width * 0.2f, playerBoundsRect.top + playerBoundsRect.height * 0.5);
};

Prize::Prize() : ChangingSprite(getPathForAsset("prize", ".png"), 0.05, {0, 0}, true, 5, 10)
// Prize::Prize() : ChangingSprite(getPathForAsset("prize", ".png"), 0.05, {0, 0})
{
    // make sure the location supports the boyancy. ALONB - also, spelling boyancy?
    setLocation(getRandomNumber(0u, static_cast<uint32_t>(dimensions::activeGameDimentions.x - getBounds().width)),
                getRandomNumber(dimensions::activeGameYOffset, static_cast<uint32_t>(dimensions::activeGameDimentions.y - getBounds().height)));

    yMid = getBounds().top;
    yDelta = 0.025;
}

void Prize::advance(float dt, int8_t x, int8_t y)
{
    // ChangingSprite::advance(dt);
    yDelta += (yMid - getBounds().top) * dt * 0.003;
    // // cout << yDelta << endl;

    if ((getBounds().top < yMid) && (getBounds().top + yDelta >= yMid))
    {
        // Fix so that the boyancy movement does not wander off..
        yDelta = 0.015;
    }
    sprite.move(0, yDelta);
    ChangingSprite::advance(dt, 0, 0); // the sprite is already advanced so just fade.
}

Player::Player() : ChangingSprite(getPathForAsset("player", ".png"), GAME_BOARD_PLAYER_X_SIZE_RATIO, {GAME_BOARD_PLAYER_SPEED_X_SCREENS_PER_SEC, GAME_BOARD_PLAYER_SPEED_Y_SCREENS_PER_SEC}) // ALONB - change these pixels per sec
{
    currentXSpeed = 0;
    currentYSpeed = 0;
    accelarationX = GAME_BOARD_PLAYER_SPEED_X_SCREENS_PER_SEC * GAME_BOARD_PLAYER_ACCELERATION_FACTOR;
    accelarationY = GAME_BOARD_PLAYER_SPEED_Y_SCREENS_PER_SEC * GAME_BOARD_PLAYER_ACCELERATION_FACTOR;
    setLocation(dimensions::activeGameDimentions.x * GAME_BOARD_PLAYER_X_OFFSET_RATIO,
                dimensions::activeGameYOffset + (dimensions::activeGameDimentions.y - getBounds().height) / 2);
};

void Player::advance(float dt, int8_t x, int8_t y)
{
    accelarationX = 4000 * dt;
    accelarationY = 3000 * dt / 1.6;
    if (x == 1)
    {
        currentXSpeed = min(currentXSpeed + accelarationX, speedPixPerSecond.x);
    }

    else if (x == -1)
    {
        currentXSpeed = max(currentXSpeed - accelarationX, -speedPixPerSecond.x);
    }

    else if (x == 0)
    {
        if (currentXSpeed > 0)
        {
            currentXSpeed = max(currentXSpeed - accelarationX, 0.0f);
        }
        else if (currentXSpeed < 0)
        {
            currentXSpeed = min(currentXSpeed + accelarationX, 0.0f);
        }
    }

    if (y == 1)
    {
        currentYSpeed = min(currentYSpeed + accelarationY, speedPixPerSecond.y);
    }

    else if (y == -1)
    {
        currentYSpeed = max(currentYSpeed - accelarationY, -speedPixPerSecond.y);
    }

    else if (y == 0)
    {
        if (currentYSpeed > 0)
        {
            currentYSpeed = max(currentYSpeed - accelarationY, 0.0f);
        }
        else if (currentYSpeed < 0)
        {
            currentYSpeed = min(currentYSpeed + accelarationY, 0.0f);
        }
    }

    float dx = dt * currentXSpeed;
    float dy = dt * currentYSpeed;

    float actualX = clamp(this->getBounds().left + dx,
                          0.0f,
                          dimensions::activeGameDimentions.x - this->getBounds().width);

    if (actualX == 0.0f || actualX == dimensions::activeGameDimentions.x - this->getBounds().width)
    {
        // hitting the wall should completely stop the movement
        currentXSpeed = 0;
    }
    float actualY = clamp(this->getBounds().top + dy,
                          static_cast<float>(dimensions::activeGameYOffset),
                          dimensions::screenDimentions.y - this->getBounds().height);

    if (actualY == static_cast<float>(dimensions::activeGameYOffset) || actualY == dimensions::screenDimentions.y - this->getBounds().height)
    {
        // hitting the wall should completely stop the movement
        currentYSpeed = 0;
    }
    sprite.setPosition(actualX, actualY);
}

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
