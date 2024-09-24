#include <SFML/Graphics.hpp>
#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <thread>
#include <vector>

using namespace std;

/* singleton class to hold the main Screen object */

Screen::Screen(float ratio)
{
    std::cout << "[Screen] - Setting up screen" << ratio << std::endl;
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    uint32_t gameXDim;
    uint32_t gameYDim;

    gameXDim = static_cast<uint32_t>(desktopMode.width * ratio);
    // y = static_cast<uint32_t>(desktopMode.height * ratio);
    gameYDim = gameXDim * 9 / 16 * ratio;

    // Print the Screen dimensions
    std::cout << "[Screen] - deskTop Width: " << desktopMode.width << std::endl;
    std::cout << "[Screen] - deskTop Height: " << desktopMode.height << std::endl;
    std::cout << "[Screen] - game Screen Width: " << gameXDim << std::endl;
    std::cout << "[Screen] - game Screen Height: " << gameYDim << std::endl;

    window.create(sf::VideoMode(gameXDim, gameYDim), "Game"); // ALONB - name?

    GameShapes *gameShapes = GameShapes::getGameShapes();
    gameShapes->setGameScreenDimensions(gameXDim, gameYDim);
}

void Screen::Render(std::promise<bool> &&promise)
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

        std::lock_guard<std::mutex> lock(gameShapes->_mutex);
        float deltaTime = clock.restart().asSeconds();
        window.clear(sf::Color::Black);

        if (gameShapes->getIsGameOver())
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
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
            // ALONB - there should be X speed and Y speed with proportions to the screen.

            gameShapes->updateMovables(deltaTime, playerSteps);
        }

        const vector<sf::Drawable *> &drawables = gameShapes->updateAndGetItemsToDraw(); // This must be refreshed.

        for (const auto &i : drawables)
        {
            window.draw(*i);
        }

        window.display();
    }
}
