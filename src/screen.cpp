#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <vector>
#include <cassert>

using namespace std;

Screen::Screen(float ratio)
{

    uint32_t gameXDim;
    uint32_t gameYDim;
    // Verify 1 time initilization for screen
    assert(!isInitialized && "screen was already initilized");
    isInitialized = true;

    std::cout << "[Screen] - Setting up screen" << ratio << std::endl;
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    gameXDim = desktopMode.width * ratio;
    gameYDim = gameXDim * ratio * GAME_SCREEN_X_TO_Y_RATIO;

    // Print the Screen dimensions
    std::cout << "[Screen] - deskTop Width: " << desktopMode.width << std::endl;
    std::cout << "[Screen] - deskTop Height: " << desktopMode.height << std::endl;
    std::cout << "[Screen] - game Screen Width: " << gameXDim << std::endl;
    std::cout << "[Screen] - game Screen Height: " << gameYDim << std::endl;

    window.create(sf::VideoMode(gameXDim, gameYDim), GAME_NAME_STRING);

    GameShapes *gameShapes = GameShapes::getGameShapes();
    gameShapes->setGameScreenDimensions(gameXDim, gameYDim);
}

uint16_t tester = 0;
void Screen::Render()
{
    GameShapes *gameShapes = GameShapes::getGameShapes();
    pair<float, float> playerSteps;
    sf::Clock clock;

    // window.setFramerateLimit(10000);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            // ALONB - what to do when this closes?
        }

        float deltaTime = clock.restart().asSeconds();
        window.clear(sf::Color::Black);

        if (gameShapes->getIsGameOver())
        {

            tester++;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || (tester == 0))
            {
                cout << "ENTER" << endl;
                // promise.set_value(true);
                gameShapes->asyncSignal->send();
                gameShapes->resetGameOver();
            }
        }

        if (!gameShapes->isCollionWithObsticle())
        {

            gameShapes->checkCollisions();

            playerSteps = {0, 0};
            playerSteps.first += sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ? -1 : 0;
            playerSteps.first += sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ? 1 : 0;
            playerSteps.second += sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ? -1 : 0;
            playerSteps.second += sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ? 1 : 0;
            // ALONB - there is a bug! the game gets stuck when a key is pressed during countdown!
            //  ALONB - there should be X speed and Y speed with proportions to the screen.

            gameShapes->updateMovables(deltaTime, playerSteps);
        }
        std::lock_guard<std::mutex> lock(gameShapes->_mutex); // The list that is generated is of pointers so canno move mutex lower than this.

        const vector<sf::Drawable *> &drawables = gameShapes->updateAndGetItemsToDraw(); // This must be refreshed.

        for (const auto &i : drawables)
        {
            window.draw(*i);
        }

        window.display();
    }
}
