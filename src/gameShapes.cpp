// ALONB - enable some warnings and errors in compiler
#include <iostream>
#include <SFML/Graphics.hpp>
#include <gameShapes.hpp>
#include <dB.hpp>
#include <mutex>
#include <cassert>

using namespace std;

unique_ptr<GameShapes> GameShapes::instance = nullptr; // ALONB - where should this be?

GameShapes *GameShapes::getGameShapes()
{
    if (instance == nullptr)
    {
        instance = make_unique<GameShapes>();
    }
    return instance.get();
}

template <typename T>
void GameShapes::addShapeCollectionToScreen(list<unique_ptr<T>> &collection, int8_t numberOfItemsToDraw) // ALONB - make these inline?
{
    if (numberOfItemsToDraw == -1)
    {

        for (const auto &i : collection)
        {
            if (!i->getIsBlink() || (i->getIsBlink() && !isBlink))
            {
                drawablesList.push_back(i->getDrawable());
            }
        }
    }
    else
    {
        auto itt = collection.begin();
        while (numberOfItemsToDraw > 0 && itt != collection.end())
        {
            if (!(*itt)->getIsBlink() || ((*itt)->getIsBlink() && !isBlink))
            {
                drawablesList.push_back((*itt)->getDrawable());
            }
            numberOfItemsToDraw--;
            itt++;
        }
    }
}

template <typename T>
void GameShapes::addSingleShapeToScreen(T &item)
{
    if (item)
    {
        if (!item->getIsBlink() || (item->getIsBlink() && !isBlink))
        {
            drawablesList.push_back(item->getDrawable());
        }
    }
}

template <typename T>
void GameShapes::simpleMoveShapeCollection(list<unique_ptr<T>> &collection, float dt)
{
    for (auto &i : collection)
    {
        i->advance(dt);
    }
}

void GameShapes::setActiveGame()
{
    cout << "[GameShapes] - setActiveGame" << endl;
    std::lock_guard<std::mutex> lock(_mutex);
    clearGameBoard();
    isCollisions = false;
    player = make_unique<Player>();
    score = make_unique<ScoreText>();
    scoreSign = make_unique<ScoresIcon>();
};

vector<sf::Drawable *> &GameShapes::updateAndGetItemsToDraw() // ALONB this could use double buffering to get the mutex out of the screen
{
    drawablesList.clear();

    int8_t livesToDraw = dB::getDB()->getLives() - 1;
    livesToDraw = max(livesToDraw - (blackout ? 1 : 0), 0);

    addShapeCollectionToScreen(lives, livesToDraw);
    addSingleShapeToScreen(scoreSign);
    addSingleShapeToScreen(score);

    if (blackout)
    {
        return drawablesList;
    }

    addShapeCollectionToScreen(gameOverTexts);
    addShapeCollectionToScreen(numCountdown);
    addShapeCollectionToScreen(extraLifeIcons);
    addShapeCollectionToScreen(bubbles);
    addSingleShapeToScreen(player);
    addShapeCollectionToScreen(prizes);
    addShapeCollectionToScreen(sharks);
    addShapeCollectionToScreen(meduzes);

    return drawablesList;
}

void GameShapes::setCountDown(uint8_t num)
{

    std::lock_guard<std::mutex> lock(_mutex);

    clearEndOfGame();
    list<unique_ptr<sf::RectangleShape>> shapes;
    switch (num)
    {
    case 1:
        shapes = simpleObjectFactory::createNum1();
        break;
    case 2:
        shapes = simpleObjectFactory::createNum2();
        break;
    case 3:
        shapes = simpleObjectFactory::createNum3();
        break;
    }

    numCountdown.clear();
    for (auto &i : shapes)
    {
        unique_ptr<CountDownNumberItem> item = make_unique<CountDownNumberItem>(*(move(i)));
        numCountdown.push_back(move(item));
    }
}

void GameShapes::updateMovables(float dt, pair<int8_t, int8_t> playerSteps)
{

    std::lock_guard<std::mutex> lock(_mutex);
    simpleMoveShapeCollection(sharks, dt);
    simpleMoveShapeCollection(meduzes, dt);
    simpleMoveShapeCollection(bubbles, dt);
    simpleMoveShapeCollection(prizes, dt);
    simpleMoveShapeCollection(extraLifeIcons, dt);
    if (player)
    {
        player->advance(dt, playerSteps.first, playerSteps.second);
    }

    // check if need to create a bubble:

    if ((playerSteps.first == 0) && (playerSteps.second == 0))
    {
        nextTimeUntilBubble = GAME_SHAPES_BUBBLE_DT_INITIAL;
        TimeUntilBubble = nextTimeUntilBubble;
    }
    else
    {
        TimeUntilBubble -= dt;
        if (TimeUntilBubble < 0)
        {
            nextTimeUntilBubble = min(nextTimeUntilBubble * GAME_SHAPES_BUBBLE_DT_DECCELERATION_FACTOR, GAME_SHAPES_BUBBLE_DT_MAX);
            TimeUntilBubble = nextTimeUntilBubble;
            createNewBubble();
        }
    }
};

void GameShapes::createNewBubble()
{
    unique_ptr<Bubble> newBubble = make_unique<Bubble>(0.8, -0.5, player->getBounds());
    bubbles.push_back(move(newBubble));
}

void GameShapes::updateScore(string score)
{
    // std::lock_guard<std::mutex> lock(_mutex);
    this->score->updateText(score);
}

void GameShapes::createNewShark()
{

    std::lock_guard<std::mutex> lock(_mutex);
    bool collisionDuringCreation;
    do
    {
        collisionDuringCreation = false;

        // float sharkXscale;
        // float sharkSpeed;

        // sharkXscale = getRandomNumber(
        //     OBJECTS_SHARK_SCALE_X * (1.0f - GAME_MANAGER_GENERAL_RANDOM_FACTOR),
        //     OBJECTS_SHARK_SCALE_X * (1.0f + GAME_MANAGER_GENERAL_RANDOM_FACTOR));

        // sharkSpeed = getRandomNumber(
        //     OBJECTS_SHARK_SPEED_X * (1.0f - GAME_MANAGER_GENERAL_RANDOM_FACTOR),
        //     OBJECTS_SHARK_SPEED_X * (1.0f + GAME_MANAGER_GENERAL_RANDOM_FACTOR));

        // sharkXscale = getRandomNumber(
        //     0.3f,
        //     0.8f);

        // cout << sharkXscale << endl;

        // sharkXscale = 0.08;

        unique_ptr<Shark> newShark = make_unique<Shark>(OBJECTS_SHARK_SCALE_X, OBJECTS_SHARK_SPEED_X);

        // make sure there is no collision

        for (const auto &i : sharks)
        {
            if (i->checkColision(newShark->getBounds()))
            {
                collisionDuringCreation = true;
                break;
            }
        }

        if (!collisionDuringCreation)
        {
            sharks.push_back(move(newShark));
        }
    } while (collisionDuringCreation);
}

void GameShapes::createNewMeduz()
{
    std::lock_guard<std::mutex> lock(_mutex);
    unique_ptr<Meduz> newMeduz = make_unique<Meduz>(OBJECTS_MEDUZA_SCALE_X, OBJECTS_MEDUZA_SPEED_Y);
    meduzes.push_back(move(newMeduz));
}

void GameShapes::createNewPrize()
{
    std::lock_guard<std::mutex> lock(_mutex);
    bool collisionDuringCreation;
    do
    {
        collisionDuringCreation = false;
        unique_ptr<Prize> newPrize = make_unique<Prize>();

        // make sure there is no collision
        for (const auto &i : prizes)
        {
            if (i->checkColision(newPrize->getBounds()))
            {
                collisionDuringCreation = true;
                break;
            }
        }

        sf::FloatRect playerClearenceArea(
            player->getBounds().left - OBJECTS_NUM_OF_PLAYER_CLEARENCE_FROM_PRIZE * player->getBounds().width,
            player->getBounds().top - OBJECTS_NUM_OF_PLAYER_CLEARENCE_FROM_PRIZE * player->getBounds().height,
            player->getBounds().width * (OBJECTS_NUM_OF_PLAYER_CLEARENCE_FROM_PRIZE * 2 + 1),
            player->getBounds().height * (OBJECTS_NUM_OF_PLAYER_CLEARENCE_FROM_PRIZE * 2 + 1));

        if (newPrize->checkColision(playerClearenceArea))
        {
            collisionDuringCreation = true;
        }

        if (!collisionDuringCreation)
        {
            prizes.push_back(move(newPrize));
        }
    } while (collisionDuringCreation);
}

void GameShapes::cleanUpOldObjects() // ALONB - explain in docu why i decided to keep these in seperate lists, it would make rendering much harder etc..
{
    std::lock_guard<std::mutex> lock(_mutex);

    sf::FloatRect screenRect{
        0 - static_cast<float>(GAME_SCREEN_PADDING_FOR_CLEANINIG),
        dimensions::activeGameYOffset -
            static_cast<float>(GAME_SCREEN_PADDING_FOR_CLEANINIG),
        dimensions::activeGameDimentions.x +
            static_cast<float>(GAME_SCREEN_PADDING_FOR_CLEANINIG * 2),
        dimensions::activeGameDimentions.y +
            static_cast<float>(GAME_SCREEN_PADDING_FOR_CLEANINIG * 2)};

    while ((sharks.size() > 0) && (!sharks.front().get()->checkColision(screenRect)))
    {
        assert(sharks.size() < 100);
        sharks.pop_front();
    }

    while ((meduzes.size() > 0) && (!meduzes.front().get()->checkColision(screenRect)))
    {
        assert(meduzes.size() < 100);
        meduzes.pop_front();
    }

    while ((bubbles.size() > 0) && (!bubbles.front().get()->checkColision(screenRect)))
    {
        assert(bubbles.size() < 100); // ALONB ADD TEXTS
        bubbles.pop_front();
    }

    // while ((prizes.size() > 0) && (prizes.front().get()->canRelease()))
    while ((prizes.size() > 0) && (!prizes.front().get()->checkColision(screenRect)))
    {
        assert(prizes.size() < 100);
        prizes.pop_front();
    }

    while ((extraLifeIcons.size() > 0) && (!extraLifeIcons.front().get()->checkColision(screenRect)))
    {
        assert(extraLifeIcons.size() < 100);
        extraLifeIcons.pop_front();
    }

    // ALONB - assert that the sizes do not explode?
}

void GameShapes::checkCollisions()
{
    std::lock_guard<std::mutex> lock(_mutex);

    for (auto &i : sharks)
    {
        if (i->checkColision(player->getBounds()))
        {
            isCollisions = true;
            break;
        }
    }

    for (auto &i : meduzes)
    {
        if (i->checkColision(player->getBounds()))
        {
            isCollisions = true;
            break;
        }
    }

    for (auto itt = prizes.begin(); itt != prizes.end();)
    {
        if (itt->get()->checkColision(player->getBounds()))
        {
            prizes.erase(itt);
            if (dB::getDB()->incrementScore(100))
            {
                cout << "INC" << endl;
                createNewLiveIcon();
            }

            // updateScore(to_string(dB::getDB()->getScore()));
            cout << "Prize" << endl;
            updateScore(to_string(dB::getDB()->getScore()));
            break;
        }
        else
        {
            itt++;
        }
    }
}

bool GameShapes::isCollionWithObsticle()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return isCollisions;
}

bool GameShapes::getIsGameOver()
{
    std::lock_guard<std::mutex> lock(_mutex);

    return isGameOver;
}

void GameShapes::setLives()
{
    std::lock_guard<std::mutex> lock(_mutex);

    cout << "Setting lives icons" << endl;

    for (int i = 0; i < dB::getDB()->getMaxLives(); i++)
    {
        // ALONB - change the way that the RegularSprite consturctor works because i have to add this 0 in the X and it's messy.
        unique_ptr<lifeIcon> life = make_unique<lifeIcon>(i);

        // Move to correct location
        lives.push_back(move(life));
    }
}

void GameShapes::flickerScreen()
{
    std::lock_guard<std::mutex> lock(_mutex);
    blackout = blackout ? false : true;
}

void GameShapes::blink()
{
    std::lock_guard<std::mutex> lock(_mutex);

    isBlink = isBlink ? false : true;
}

void GameShapes::clearGameBoard()
{

    numCountdown.clear();
    extraLifeIcons.clear();
    bubbles.clear();
    player.release();
    prizes.clear();
    sharks.clear();
    meduzes.clear();
}

void GameShapes::clearEndOfGame()
{
    gameOverTexts.clear();
    lives.clear();
    scoreSign.release();
    score.release();
}

void GameShapes::gameOver(uint32_t score, bool isHighScore)
{
    std::lock_guard<std::mutex> lock(_mutex);

    isGameOver = true;
    // this->score->updateText(score);

    unique_ptr<GameOverText> gameOverText = make_unique<GameOverText>();
    unique_ptr<ScoresText> ScoreCountText = make_unique<ScoresText>(gameOverText->getBounds().height * 2, 500, 500);
    unique_ptr<pressEnterToRestart> pressEnterText = make_unique<pressEnterToRestart>();

    gameOverTexts.push_back(move(gameOverText));
    gameOverTexts.push_back(move(ScoreCountText));
    gameOverTexts.push_back(move(pressEnterText));

    clearGameBoard();
}

void GameShapes::resetGameOver()
{
    std::lock_guard<std::mutex> lock(_mutex);

    isGameOver = false;
}

void GameShapes::createNewLiveIcon()
{
    unique_ptr<ExtraLifeIcon> newLifeIcon = make_unique<ExtraLifeIcon>();
    extraLifeIcons.push_back(move(newLifeIcon));
}

void GameShapes::setGameScreenDimensions(uint32_t x, uint32_t y)
{
    dimensions::screenDimentions.x = x;
    dimensions::screenDimentions.y = y;

    dimensions::activeGameYOffset = y * GAME_SCREEN_WALL_WIDTH_RATIO;

    dimensions::activeGameDimentions.x = x;
    dimensions::activeGameDimentions.y = dimensions::screenDimentions.y - dimensions::activeGameYOffset;
}
