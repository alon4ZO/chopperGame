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

    uint32_t x;
    uint32_t y; // ALONB - optimize

    x = static_cast<uint32_t>(desktopMode.width * ratio);
    // y = static_cast<uint32_t>(desktopMode.height * ratio);
    y = x * 9 / 16 * ratio;

    // Print the Screen dimensions
    std::cout << "[Screen] - deskTop Width: " << desktopMode.width << std::endl;
    std::cout << "[Screen] - deskTop Height: " << desktopMode.height << std::endl;
    std::cout << "[Screen] - game Screen Width: " << x << std::endl;
    std::cout << "[Screen] - game Screen Height: " << y << std::endl;

    window.create(sf::VideoMode(x, y), "Game");

    // Create the game shapes class, in order to set the dimensions.
    GameShapes *gameShapes = GameShapes::getGameShapes();
    gameShapes->setGameScreenDimensions(x, y);
}

void Screen::Render()
{
    GameShapes *gameShapes = GameShapes::getGameShapes();
    pair<float, float> playerSteps;

    // window.draw(rectangle);
    // auto &obsticles = gameShapes->getobsticals();
    // auto &countDown = gameShapes->getCountDown();
    // auto &chopper = gameShapes->getchopper();
    // auto &blackBackground = gameShapes->emptyBlack();

    sf::Clock clock;

    const float speed = 700.0f; // pixels per second

    // cout << "SETUP" << endl;

    std::cout << "[Screen] - Setting up screen" << std::endl;
    // sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    window.setFramerateLimit(10000);

    sf::Texture texture;
    if (!texture.loadFromFile("C:\\ws\\chopperGame\\pic\\player.png"))
    { // Replace with your image file path
        std::cerr << "Error loading texture!" << std::endl;
        // return -1; // Exit if there's an error
    }

    // Create a sprite
    sf::Sprite sprite;
    sprite.setTexture(texture);

    sf::Drawable *d;

    // Optionally set the position of the sprite
    // sprite.setPosition(100, 100); // Move the sprite to (100, 100)

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        std::lock_guard<std::mutex> lock(gameShapes->_mutex);
        window.clear(sf::Color::Black);

        float deltaTime = clock.restart().asSeconds();

        // check collision

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
        // }
        // if (drawables.size() == 0)
        // {
        //     cout << "InLoop " << drawables.size() << endl;
        // }

        for (const auto &i : drawables)
        {
            // cout << "drawA " << endl;
            // i->draw();

            window.draw(*i);
            // window.draw(*static_cast<sf::Sprite *>(i));
            // d = &sprite;
            // window.draw(*d);
            // cout << "drawB " << endl;
        }

        // cout << "draw " << endl;
        // Set the scale proportionally

        window.display();
    }
}

// if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
// {
//     i->move(speed * deltaTime, 0.0f);
// }

// if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
// {
//     i->move(0.0f, speed * deltaTime);
// }

//  {
//         bitmap |= 1; // ALONB - BITMAP FOR THESE

//         i->move(-speed * deltaTime, 0.0f);
//     }

//     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
//     {
//         bitmap |= 2;

//         i->move(speed * deltaTime, 0.0f);
//     }

//     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
//     {
//         bitmap |= 4;

//         i->move(0.0f, -speed * deltaTime);
//     }

//     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
//     {
//         bitmap |= 8;

//         i->move(0.0f, speed * deltaTime);
//     }