#include <iostream>

int main()
{
    std::cout << "Hello, World! This is a CPP test" << std::endl;
    return 0;
}
// #include <SFML/Graphics.hpp>
// #include <SFML/System.hpp>
// #include <SFML/Window.hpp>

// #include <iostream>

// int main()
// {
//     std::cout << "HELLO" << std::endl;

//     sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

//     // Retrieve the width and height of the screen
//     unsigned int screenWidth = desktopMode.width;
//     unsigned int screenHeight = desktopMode.height;

//     // Print the screen dimensions
//     std::cout << "Screen width: " << screenWidth << std::endl;
//     std::cout << "Screen height: " << screenHeight << std::endl;

//     sf::RenderWindow window(sf::VideoMode(screenWidth / 2, screenHeight / 2), "SFML window");

//     sf::RectangleShape rectangle;
//     rectangle.setSize(sf::Vector2f(50, 50));
//     rectangle.setOutlineColor(sf::Color::Red);
//     rectangle.setFillColor(sf::Color::Blue);
//     rectangle.setOutlineThickness(30);
//     rectangle.setPosition(25, 25);

//     window.draw(rectangle);

//     while (window.isOpen())
//     {
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }

//         window.clear(sf::Color::Black);
//         window.draw(rectangle);
//         window.display();
//     }

//     return 0;
// }
