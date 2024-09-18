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
    // /itemsToDraw.clear();

    // rest player position ALONB
}

void GameShapes::setActiveGame()
{
    clearAll(); // ALONB - make this private?
    cout << "[GameShapes] - setActiveGame" << endl;

    isCollisions = {false, false};

    std::lock_guard<std::mutex> lock(_mutex);
    drawablesList.clear();

    player = make_unique<Player>(GAME_BOARD_PLAYER_X_SIZE_RATIO);
};

vector<sf::Drawable *> &GameShapes::updateAndGetItemsToDraw()
{
    // cout << "[screen] - update" << endl;

    drawablesList.clear();

    if (blackout)
    {
        return drawablesList;
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

    cout << sharks.size() << endl;
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
            cout << "COLS" << endl;
            break;
        }
    }

    for (auto &i : meduzes)
    {
        if (i->checkColision(player->getBounds()))
        {
            cout << "COLM" << endl;
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
