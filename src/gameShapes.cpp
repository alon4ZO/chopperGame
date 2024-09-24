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

// GameShapes::GameShapes()
// {
//     moveablesList.push_back(static_player);
//     moveablesList.push_back(sharks);
//     moveablesList.push_back(meduzes);
//     moveablesList.push_back(bubbles);
// }

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

    // ALONB - this should only clear player and obsticles, change name accordingly.
    // /itemsToDraw.clear();

    // rest player position ALONB
}

void GameShapes::setActiveGame(uint8_t lives)
{
    clearAll(); // ALONB - make this private?
    cout << "[GameShapes] - setActiveGame" << endl;

    isCollisions = {false, false};

    std::lock_guard<std::mutex> lock(_mutex);
    drawablesList.clear(); // ALONB - this clears the score as well, maybe it should stay!

    player = make_unique<Player>(GAME_BOARD_PLAYER_X_SIZE_RATIO);
    score = make_unique<ScoreText>();
    scoreSign = make_unique<RegularSprite>(shapeFactory::getPathForPng("score_sign", ".png"), 0.03);
    scoreSign->setLocation(score->getBounds().left - 1.5 * scoreSign->getBounds().width, (dimensions::activeGameYOffset * 1.1 - scoreSign->getBounds().height) / 2);

    livesToDraw = lives - 1;

    // sf::Font font;
    // font.loadFromFile("arial.ttf");
    // Create a tex_si
    //     sf::Text text("hello", font);
    //     text.setCharacterSize(30);
    //     text.setStyle(sf::Text::Bold);
    //     text.setFillColor(sf::Color::Red);
    // };
};
vector<sf::Drawable *> &GameShapes::updateAndGetItemsToDraw()
{
    // cout << "[screen] - update" << endl;

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

    int8_t livesToDrawNow = livesToDraw - (blackout ? 1 : 0);
    auto itt = lives.begin();

    while (livesToDrawNow > 0)
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

    for (auto &i : sharks)
    {
        i->advance(dt);
    }
    for (auto &i : meduzes)
    {
        i->advance(dt);
    }

    if (player)
    {
        player->advance(dt, playerSteps.first, playerSteps.second);
    }
};

void GameShapes::updateScore(string score)
{
    // cout << "IN" << score << endl;

    std::lock_guard<std::mutex> lock(_mutex);

    this->score->updateText(score);
}

void GameShapes::createNewShark()
{

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
            // cout << "SOL" << endl;
        }

    } while (collisionDuringCreation);
}

void GameShapes::cleanUpOldObjects() // ALONB - make this: cleanup movables, and there should be an easily accessible list of movables.
{
    std::lock_guard<std::mutex> lock(_mutex);

    // use get Bounds insted of manual calc can use the check collision with the screen

    sf::FloatRect screenRect{0 - static_cast<float>(100), dimensions::activeGameYOffset - static_cast<float>(100), dimensions::activeGameDimentions.x + static_cast<float>(200), dimensions::activeGameDimentions.y + static_cast<float>(200)};

    // cout << sharks.size() << endl;
    // cout << "M" << meduzes.size() << endl;
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

    // ALONB - assert that the sizes do not explode?
}

void GameShapes::checkCollisions()
{

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

    // for (auto &i : obsticals)
    // {
    //     if (chopper.front()->getGlobalBounds().intersects(i->getGlobalBounds()))
    //     {
    //         isCollisions.first = true;
    //         return; // ALONB - add extra life.
    //     }
    // }
}

void GameShapes::setLives(uint8_t num)
{

    std::lock_guard<std::mutex> lock(_mutex);
    uint32_t firstLiveLocation = dimensions::screenDimentions.x * 0.1;

    cout << "Setting lives icons" << endl;

    for (int i = 0; i < num; i++)
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
    // std::lock_guard<std::mutex> lock(_mutex);

    isGameOver = false;
}

// void GameShapes::resetAsyncSignal()
// {
//     // std::lock_guard<std::mutex> lock(_mutex);

//     std::promise<bool> promiseObj = make_unique<>;
//     std::future<bool> futureObj = promiseObj.get_future();

//     isGameOver = false;
// }
