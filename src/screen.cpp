#include <SFML/Graphics.hpp>
#include <iostream>
#include <screen.hpp>

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

void Screen::play()
{
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(50, 50));
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setFillColor(sf::Color::Blue);
    rectangle.setOutlineThickness(30);
    rectangle.setPosition(25, 25);

    window.draw(rectangle);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(rectangle);
        window.display();
    }
}