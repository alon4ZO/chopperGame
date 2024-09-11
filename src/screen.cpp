#include <SFML/Graphics.hpp>
#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <thread>

using namespace std;

/* singleton class to hold the main Screen object */
unique_ptr<Screen> Screen::instance = nullptr;

Screen::Screen(uint8_t xRatioPercentage, uint8_t yRatioPercentage)
{
    std::cout << "[Screen] - Setting up screen" << std::endl;
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    gameScreenWidth = static_cast<uint32_t>(desktopMode.width * (static_cast<float>(xRatioPercentage) / 100));
    gameScreenHeight = static_cast<uint32_t>(desktopMode.height * (static_cast<float>(yRatioPercentage) / 100));

    // Print the Screen dimensions
    std::cout << "[Screen] - deskTop Width: " << desktopMode.width << std::endl;
    std::cout << "[Screen] - deskTop Height: " << desktopMode.height << std::endl;
    std::cout << "[Screen] - game Screen Width: " << gameScreenWidth << std::endl;
    std::cout << "[Screen] - game Screen Width: " << gameScreenHeight << std::endl;

    window.create(sf::VideoMode(gameScreenWidth, gameScreenHeight), "Game");

    // update the game shapes class:

    GameShapes *gameShapes = GameShapes::getGameShapes();
    gameShapes->setGameScreenDimensions(gameScreenWidth, gameScreenHeight);
}

Screen *Screen::getScreen()
{
    if (instance == nullptr)
    {
        instance = make_unique<Screen>();
        cout << "[Screen] Setting up Screen" << endl;
    }
    return instance.get();
}

void Screen::Render()
{
    GameShapes *gameShapes = GameShapes::getGameShapes();
    pair<float, float> steps;

    // window.draw(rectangle);
    auto &walls = gameShapes->getWalls();
    auto &obsticles = gameShapes->getobsticals();
    auto &countDown = gameShapes->getCountDown();
    auto &chopper = gameShapes->getchopper();

    sf::Clock clock;

    const float speed = 700.0f; // pixels per second

    sf::Texture texture;
    if (!texture.loadFromFile("C:\\ws\\chopperGame\\pic\\Cloud.png"))
    {
        std::cerr << "Error loading image!" << std::endl;
        return; // Exit if the image fails to load
    }

    // Create a sprite from the texture
    sf::Sprite sprite(texture);
    sprite.setPosition(200, 150); // Set position in the window

    float desiredWidth = gameScreenWidth / 4;   // Example desired width in pixels
    float desiredHeight = gameScreenHeight / 4; // Example desired height in pixels

    // Calculate scale factors
    sf::Vector2f originalSize(texture.getSize());

    float scaleX = desiredWidth / originalSize.x;
    float scaleY = desiredHeight / originalSize.y;

    sf::Texture texture2;
    if (!texture2.loadFromFile("C:\\ws\\chopperGame\\pic\\Sky.png"))
    {
        std::cerr << "Error loading image!" << std::endl;
        return; // Exit if the image fails to load
    }

    // Create a sprite from the texture
    sf::Sprite sprite2(texture2);
    sprite2.setPosition(0, 0); // Set position in the window

    float desiredWidth2 = gameScreenWidth;   // Example desired width in pixels
    float desiredHeight2 = gameScreenHeight; // Example desired height in pixels

    // Calculate scale factors
    sf::Vector2f originalSize2(texture2.getSize());

    float scaleX2 = desiredWidth2 / originalSize2.x;
    float scaleY2 = desiredHeight2 / originalSize2.y;

    window.setFramerateLimit(10000);

    sprite.setScale(scaleX, scaleY);
    sprite2.setScale(scaleX2, scaleY2);

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
        gameShapes->updateObsicles(deltaTime);

        window.draw(sprite2);
        sprite.setColor(sf::Color(255, 255, 255, 120)); // Set to white color with transparency

        window.draw(sprite);

        for (auto &i : obsticles)
        {
            window.draw(*i);
        }

        for (auto &i : walls)
        {
            window.draw(*i);
        }

        for (auto &i : countDown)
        {
            window.draw(*i);
        }

        for (auto &i : chopper)
        {
            window.draw(*i);
        }

        steps = {0, 0};
        steps.first += sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ? -speed * deltaTime : 0;
        steps.first += sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ? speed * deltaTime : 0;
        steps.second += sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ? -speed * deltaTime : 0; // ALONB - there should be X speed and Y speed with proportions to the screen.
        steps.second += sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ? speed * deltaTime : 0;

        gameShapes->moveChopper(steps);

        // Desired pixel size

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