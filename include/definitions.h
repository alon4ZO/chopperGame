#pragma once

#include <random>
// General
#define GAME_BOARD_COUNTDOWN_TIME_INTERVALS_MS (500)

// graphics:
#define GAME_BOARD_PLAYER_X_OFFSET_RATIO (0.1)
#define GAME_BOARD_RATIO (0.8)
// #define GAME_BOARD_WALL_WIDTH_RATIO (0.2) // Factor
#define GAME_BOARD_WALL_WIDTH_RATIO (0.15) // Factor

#define GAME_BOARD_PLAYER_SPEED_X_SCREENS_PER_SEC (0.6) // Factor
#define GAME_BOARD_PLAYER_SPEED_Y_SCREENS_PER_SEC (0.6) // Factor

// #define GAME_BOARD_WALL_COLORS (24, 84, 173)

#define NUMBER_HEIGHT_RATIO (3)

#define MANAGER_INITIAL_SCORE (0)
#define MANAGER_INITIAL_OBSTICLE_SPEED_PIX_PER_SEC (5)
// #define MANAGER_INITIAL_SHAPES_PER_SEC (9) NOT BAD
#define MANAGER_INITIAL_SHARKS_PER_SEC (1.5)

// ALONB - this shouldn't be here
template <class T>
static uint32_t getRandomNumber(T min, T max) // ALONB - how could this class own some of the dimentions?
{
    // Create a random number generator and seed it with a random device
    std::random_device rd;
    std::mt19937 eng(rd());

    // Create a uniform distribution in the specified range
    if constexpr (std::is_integral<T>::value)
    {
        // For integral types
        std::uniform_int_distribution<T> dist(min, max);
        return dist(eng);
    }
    else if constexpr (std::is_floating_point<T>::value)
    {
        // For floating-point types
        std::uniform_real_distribution<T> dist(min, max);
        return dist(eng);
    }
    else
    {
        static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value,
                      "Type must be an integral or floating-point type.");
    }

    // return eng;
}