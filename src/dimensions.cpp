#include <SFML/Graphics.hpp>
#include <iostream>
#include <screen.hpp>
#include <gameShapes.hpp>
#include <manager.hpp>
#include <dimensions.hpp>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::literals;

sf::Vector2u dimensions::screenDimentions;     // Definition
sf::Vector2u dimensions::activeGameDimentions; // Definition
uint32_t dimensions::activeGameYOffset;        // Definition (initialize if necessary)