#include <SFML/Graphics.hpp>
#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>

using namespace std;

/* singleton class to hold the main Screen object */
unique_ptr<Screen> Screen::instance = nullptr;

Screen::Screen(uint8_t xRatioPercentage, uint8_t yRatioPercentage)
{
    std::cout << "Setting up screen" << std::endl;
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    gameScreenWidth = static_cast<uint32_t>(desktopMode.width * (static_cast<float>(xRatioPercentage) / 100));
    gameScreenHeight = static_cast<uint32_t>(desktopMode.height * (static_cast<float>(yRatioPercentage) / 100));

    // Print the Screen dimensions
    std::cout << "deskTop Width: " << desktopMode.width << std::endl;
    std::cout << "deskTop Height: " << desktopMode.height << std::endl;
    std::cout << "game Screen Width: " << gameScreenWidth << std::endl;
    std::cout << "game Screen Width: " << gameScreenHeight << std::endl;

    window.create(sf::VideoMode(gameScreenWidth, gameScreenHeight), "Game");
}

Screen *Screen::getScreen()
{
    if (instance == nullptr)
    {
        instance = make_unique<Screen>();
        cout << "Setting up Screen" << endl;
    }
    return instance.get();
}

void Screen::play(const GameShapes &shapes)
{

    // window.draw(rectangle);
    auto &walls = shapes.getWalls();
    auto &obsticles = shapes.getobsticals();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        for (auto &i : walls)
        {
            window.draw(*i);
        }
        for (auto &i : obsticles)
        {
            window.draw(*i);
        }
        window.display();
    }
}