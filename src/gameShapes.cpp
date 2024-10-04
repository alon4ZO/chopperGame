// ALONB - enable some warnings and errors in compiler
#include <SFML/Graphics.hpp>
#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <dB.hpp>
#include <mutex>
#include <filesystem>
#include <definitions.h>

using namespace std;

unique_ptr<GameShapes> GameShapes::instance = nullptr;

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
    std::lock_guard<std::mutex> lock(_mutex);

    sharks.clear();
    meduzes.clear();
    numCountdown.clear();
    bubbles.clear();
    player.release();
    prizes.clear();
    extraLifeIcons.clear();

    // ALONB - this should only clear player and obsticles, change name accordingly.
    // /itemsToDraw.clear();

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
    scoreSign = make_unique<RegularSprite>(shapeFactory::getPathForPng("score_sign", ".png"), 0.03);
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

    numCountdown = move(numShapes);
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
            // float scale = 1 - ((nextTimeUntilBubble - 0.01) / (1.0f - 0.01));
            float scale = 1;
            createNewBubble(scale); // ALONB - do I want different scales?
        }
    }
};

void GameShapes::createNewBubble(float scale)
{
    // cout << scale << endl;
    unique_ptr<Bubble> newBubble = make_unique<Bubble>(0.8 * scale, -0.5, player->getBounds());
    bubbles.push_back(move(newBubble));
    // cout << bubbles.size() << endl;
}

void GameShapes::updateScore(string score)
{
    // cout << "IN" << score << endl;

    std::lock_guard<std::mutex> lock(_mutex); // ALONB - need to make sense of all these mutexes.

    this->score->updateText(score);
}

void GameShapes::createNewShark()
{

    std::lock_guard<std::mutex> lock(_mutex);
    bool collisionDuringCreation;
    do
    {
        collisionDuringCreation = false;
        unique_ptr<Shark> newShark = make_unique<Shark>(0.08, -0.25);

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
            sharks.push_back(move(newShark));
            // cout << "SOL" << endl;
        }

    } while (collisionDuringCreation);
}

void GameShapes::createNewMeduz() // ALONB the meduzes can collide?
{
    // ALONB - add a non colliding private function to be used by meduz, shark.
    std::lock_guard<std::mutex> lock(_mutex);
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
            meduzes.push_back(move(newMeduz));
            // cout << "SOL" << endl;
        }

    } while (collisionDuringCreation);
}

void GameShapes::createNewPrize()
{

    std::lock_guard<std::mutex> lock(_mutex);
    bool collisionDuringCreation;
    do
    {
        collisionDuringCreation = false;
        unique_ptr<Prize> newPrize = make_unique<Prize>();

        // MAKE SURE DOES NOT COLLIDE WITH OTHER SHARKS

        for (const auto &i : prizes)
        {
            // if (i->checkColision(newShark.get()->getBounds()))
            if (i->checkColision(newPrize->getBounds()))
            {
                collisionDuringCreation = true;
                // cout << "COL" << endl;
                break;
            }
        }

        // ALONB this can also be for sharks, meduz, doesn't have to - optional
        sf::FloatRect playerClearenceArea(player->getBounds().left - player->getBounds().width,
                                          player->getBounds().top - player->getBounds().height,
                                          player->getBounds().width * 3,
                                          player->getBounds().height * 3);

        if (newPrize->checkColision(playerClearenceArea))
        {
            collisionDuringCreation = true;
        }

        if (!collisionDuringCreation)
        {
            prizes.push_back(move(newPrize));
            // cout << "SOL" << endl;
        }
    } while (collisionDuringCreation);
}
// ALONB the meduzes can collide?

void GameShapes::cleanUpOldObjects() // ALONB - make this: cleanup movables, and there should be an easily accessible list of movables.
{
    std::lock_guard<std::mutex> lock(_mutex);

    // use get Bounds insted of manual calc can use the check collision with the screen

    sf::FloatRect screenRect{0 - static_cast<float>(100), dimensions::activeGameYOffset - static_cast<float>(100), dimensions::activeGameDimentions.x + static_cast<float>(200), dimensions::activeGameDimentions.y + static_cast<float>(200)};

    while ((sharks.size() > 0) && (!sharks.front().get()->checkColision(screenRect)))
    {
        sharks.pop_front();
        // cout << "POP" << endl;
    }

    while ((meduzes.size() > 0) && (!meduzes.front().get()->checkColision(screenRect)))
    {
        meduzes.pop_front();
        // cout << "POPm" << endl;
    }

    while ((bubbles.size() > 0) && (!bubbles.front().get()->checkColision(screenRect)))
    {
        bubbles.pop_front();
        // cout << "POPm" << endl;
    }

    while ((prizes.size() > 0) && (prizes.front().get()->canRelease()))
    {
        cout << "POP prize" << endl;
        prizes.pop_front();
    }
    cout << "DGB4" << endl;

    while ((extraLifeIcons.size() > 0) && (!extraLifeIcons.front().get()->checkColision(screenRect)))
    {
        cout << "POP extraLifeIcons" << endl;
        extraLifeIcons.pop_front();
    }

    cout << "DGB5" << endl;

    // ALONB - assert that the sizes do not explode?
}

void GameShapes::checkCollisions()
{
    std::lock_guard<std::mutex> lock(_mutex);

    for (auto &i : sharks)
    {
        if (i->checkColision(player->getBounds()))
        {
            isCollisions.first = true;
            // cout << "COLS" << endl;
            break;
        }
    }

    for (auto &i : meduzes)
    {
        if (i->checkColision(player->getBounds()))
        {
            // cout << "COLM" << endl;
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
            this->score->updateText(to_string(dB::getDB()->getScore()));
            break;
        }
        else
        {
            itt++;
        }
    }
}

void GameShapes::setLives()
{
    std::lock_guard<std::mutex> lock(_mutex);
    uint32_t firstLiveLocation = dimensions::screenDimentions.x * 0.1;

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

    //     unique_ptr<sf::Shape> shape = shapeFactory::createEmptyBlack(screenDimentions.x, screenDimentions.y, wallThickness); // ALONB - ad this level, they should all be called shape or something generic.
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
