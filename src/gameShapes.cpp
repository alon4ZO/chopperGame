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

void GameShapes::clearAll()
{
    std::lock_guard<std::mutex> lock(_mutex);

    sharks.clear();
    meduzes.clear();
    numCountdown.clear();
    bubbles.clear();
    player.release();
    prizes.clear();
    extraLifeIcons.clear();

    // ALONB: where are all of these?
    //  unique_ptr<Player> player;
    //  list<unique_ptr<Shark>> sharks;
    //  list<unique_ptr<Meduz>> meduzes;
    //  list<unique_ptr<Bubble>> bubbles;
    //  list<unique_ptr<sf::RectangleShape>> numCountdown;
    //  unique_ptr<ScoreText> score;
    //  unique_ptr<RegularSprite> scoreSign; // ALONB - maybe some of these should be static
    //  list<unique_ptr<RegularSprite>> lives;
    //  list<unique_ptr<GeneralText>> gameOverTexts;
    //  list<unique_ptr<Prize>> prizes;
    //  list<unique_ptr<ExtraLifeIcon>> extraLifeIcons;

    // rest player position ALONB
}

void GameShapes::setActiveGame()
{
    clearAll(); // ALONB - make this private?
    cout << "[GameShapes] - setActiveGame" << endl;

    isCollisions = {false, false};

    std::lock_guard<std::mutex> lock(_mutex);
    drawablesList.clear(); // ALONB - this clears the score as well, maybe it should stay!

    player = make_unique<Player>(GAME_BOARD_PLAYER_X_SIZE_RATIO, GAME_BOARD_PLAYER_X_SIZE_RATIO * 0.1);
    score = make_unique<ScoreText>();
    scoreSign = make_unique<RegularSprite>(ObjectFactory::getPathForPng("score_sign", ".png"), 0.03);
    scoreSign->setLocation(score->getBounds().left - 1.5 * scoreSign->getBounds().width, (dimensions::activeGameYOffset * 1.1 - scoreSign->getBounds().height) / 2);
    cout << "[GameShapes] - setActiveGame DONE" << endl;

    // livesToDraw = lives - 1;
};
vector<sf::Drawable *> &GameShapes::updateAndGetItemsToDraw()
{
    // cout << "[screen] - update" << endl;
    // std::lock_guard<std::mutex> lock(_mutex);

    drawablesList.clear(); // ALONB - maybe i should have a class of screens, each has it's own set of drawables, blinking drawables

    if (score)
    {
        // cout << "T" << endl;
        drawablesList.push_back(score->getDrawable());
    }

    if (scoreSign)
    {
        // cout << "T" << endl;
        drawablesList.push_back(scoreSign->getDrawable());
    }

    if (isGameOver)
    {
        // cout << gameOverTexts.size() << endl;
        for (const auto &i : gameOverTexts)
        {
            // cout << std::boolalpha << isBlink << endl;
            // if (!i->getIsBlink())
            if (!i->getIsBlink() || (i->getIsBlink() && !isBlink))
            {
                drawablesList.push_back(i->getDrawable());
            }
        }
        return drawablesList;
    }

    // auto it = lives.begin();
    // int8_t livesNoBlink = livesToDraw - 1;

    // while (livesNoBlink >= 0)
    // {
    //     it++;
    //     livesNoBlink--;
    //     // itemsToDraw.push_back(static_cast<sf::Drawable *>(i.get()));
    //     drawablesList.push_back((*it)->getDrawable()); // ALONB - use a shared pointer instead?
    // }

    livesToDraw = dB::getDB()->getLives() - 1;

    int8_t livesToDrawNow = livesToDraw - (blackout ? 1 : 0);
    auto itt = lives.begin();

    while (livesToDrawNow > 0 && itt != lives.end())
    {
        // cout << "EEEEEEEEEEEEEEEEE" << endl;
        drawablesList.push_back((*itt)->getDrawable()); // ALONB - use a shared pointer instead?
        livesToDrawNow--;
        itt++;
    }

    // cout << "dddd" << endl;

    if (blackout)
    {
        return drawablesList;
    }

    // drawablesList.push_back(it->getDrawable()); // ALONB - use a shared pointer instead?

    for (const auto &i : extraLifeIcons)
    {
        // itemsToDraw.push_back(static_cast<sf::Drawable *>(i.get()));
        drawablesList.push_back(i->getDrawable()); // ALONB - use a shared pointer instead?
    }

    for (const auto &i : bubbles)
    {
        // itemsToDraw.push_back(static_cast<sf::Drawable *>(i.get()));
        drawablesList.push_back(i->getDrawable()); // ALONB - use a shared pointer instead?
    }

    if (player)
    {
        // cout << "T" << endl;
        drawablesList.push_back(player->getDrawable());
    }

    for (const auto &i : numCountdown)
    {
        // itemsToDraw.push_back(static_cast<sf::Drawable *>(i.get()));
        drawablesList.push_back(i.get()); // ALONB - use a shared pointer instead?
    }

    for (const auto &i : prizes)
    {
        // itemsToDraw.push_back(static_cast<sf::Drawable *>(i.get()));
        drawablesList.push_back(i->getDrawable()); // ALONB - use a shared pointer instead?
    }

    for (const auto &i : sharks)
    {
        // itemsToDraw.push_back(static_cast<sf::Drawable *>(i.get()));
        drawablesList.push_back(i->getDrawable()); // ALONB - use a shared pointer instead?
    }

    for (const auto &i : meduzes)
    {
        // itemsToDraw.push_back(static_cast<sf::Drawable *>(i.get()));
        drawablesList.push_back(i->getDrawable()); // ALONB - use a shared pointer instead?
    }

    return drawablesList;
}

void GameShapes::setCountDown(uint8_t num)
{
    std::lock_guard<std::mutex> lock(_mutex);
    list<unique_ptr<sf::RectangleShape>> shapes;
    switch (num)
    {
    case 1:
        shapes = ObjectFactory::createNum1();
        break;
    case 2:
        shapes = ObjectFactory::createNum2();
        break;
    case 3:
        shapes = ObjectFactory::createNum3();
        break;
    }

    numCountdown = move(shapes);
}

void GameShapes::updateMovables(float dt, pair<int8_t, int8_t> playerSteps) // ALONB add inputs here.
{

    std::lock_guard<std::mutex> lock(_mutex);

    for (auto &i : sharks)
    {
        i->advance(dt);
    }

    for (auto &i : meduzes)
    {
        i->advance(dt);
    }

    for (auto &i : bubbles)
    {
        i->advance(dt);
    }

    for (auto &i : prizes)
    {
        i->advance(dt);
    }

    for (auto &i : extraLifeIcons)
    {
        i->advance(dt);
    }

    if (player)
    {
        player->advance(dt, playerSteps.first, playerSteps.second);
    }

    // check if need to create a bubble:

    if ((playerSteps.first == 0) && (playerSteps.second == 0))
    {
        nextTimeUntilBubble = 0.01;
        TimeUntilBubble = nextTimeUntilBubble;
    }
    else
    {
        TimeUntilBubble -= dt;
        if (TimeUntilBubble < 0)
        {
            nextTimeUntilBubble = min(nextTimeUntilBubble * 1.8f, 1.0f);
            TimeUntilBubble = nextTimeUntilBubble;
            float scale = 1;
            createNewBubble(scale); // ALONB - do I want different scales?
        }
    }
};

void GameShapes::createNewBubble(float scale)
{
    unique_ptr<Bubble> newBubble = make_unique<Bubble>(0.8 * scale, -0.5, player->getBounds());
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
            isCollisions.first = true; // ALONB - no pair needed here
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
    return isCollisions.first;
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

    // if (blackout.size() == 1)
    // {
    //     blackout.pop_back();
    // }
    // else
    // {

    //     unique_ptr<sf::Shape> shape = ObjectFactory::createEmptyBlack(screenDimentions.x, screenDimentions.y, wallThickness); // ALONB - ad this level, they should all be called shape or something generic.
    //     std::lock_guard<std::mutex> lock(_mutex);
    //     blackout.push_back(move(shape));
    // }
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
