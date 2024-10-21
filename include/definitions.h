#pragma once

#include <random>

// Name
#define GAME_NAME_STRING ("shark")

// Game screen
#define GAME_SCREEN_X_FROM_HOST_SCREEN_RATIO (0.8)
#define GAME_SCREEN_X_TO_Y_RATIO (9.0f / 16)
#define GAME_SCREEN_WALL_WIDTH_RATIO (0.15)
#define GAME_SCREEN_PADDING_FOR_CLEANINIG (200)

// Game management
#define GAME_MANAGER_GENERAL_RANDOM_FACTOR (0.3f)
#define GAME_MANAGER_COUNTDOWN_START_NUM (2)
#define GAME_MANAGER_FLICKERS_WHEN_COLLIDE (4)
#define GAME_MANAGER_MEDUZA_COUNTDOWN_TIME_MS (5000)
#define GAME_MANAGER_PRIZE_COUNTDOWN_TIME_MS (20000)
#define GAME_MANAGER_SHARKS_PER_PRIZE (0.3f)
#define MANAGER_INITIAL_SHARKS_PER_SECOND (0.7)
#define GAME_MANAGER_SHARKS_FLICKER_TIME_MS (150)
#define GAME_OVER_BLINK_TIME_MS (500)

// data base
#define DB_NUM_OF_INITIAL_LIVES (2)
#define DB_MAX_LIVES (5)
#define DB_SCORE_PER_EXTRA_LIFE (50)
#define DB_SCORE_SCORE_FOR_NEW_SHARK (10)
#define DB_SCORE_SCORE_FOR_NEW_MEDUZA (15)

// Objects:
#define OBJECTS_SHARK_SCALE_X (0.08f)
#define OBJECTS_SHARK_SPEED_X (-0.25f)

#define OBJECTS_MEDUZA_SCALE_X (0.1f)
#define OBJECTS_MEDUZA_SPEED_Y (-0.25f)

#define OBJECTS_NUM_OF_PLAYER_CLEARENCE_FROM_PRIZE (2)

#define GAME_BOARD_COUNTDOWN_TIME_INTERVALS_MS (100)

// graphics:
#define GAME_BOARD_PLAYER_X_OFFSET_RATIO (0.1)

#define GAME_BOARD_PLAYER_X_SIZE_RATIO (0.05)
#define GAME_BOARD_PLAYER_ACCELERATION_FACTOR (GAME_BOARD_PLAYER_X_SIZE_RATIO * 0.1)
#define GAME_BOARD_BUBBLE_X_SCALE (0.015)

#define GAME_BOARD_PLAYER_SPEED_X_SCREENS_PER_SEC (0.6) // Factor
#define GAME_BOARD_PLAYER_SPEED_Y_SCREENS_PER_SEC (0.6) // Factor

#define GAME_BOARD_EXTRA_LIFE_ICON_SPEED_Y_SCREENS_PER_SEC (-0.3)
#define GAME_BOARD_EXTRA_LIFE_ICON_X_RATIO (0.05)

// #define GAME_BOARD_WALL_COLORS (24, 84, 173)

#define NUMBER_HEIGHT_RATIO (3)

#define MANAGER_INITIAL_SCORE (0)
#define MANAGER_INITIAL_OBSTICLE_SPEED_PIX_PER_SEC (5)
// #define MANAGER_INITIAL_SHAPES_PER_SEC (9) NOT BAD
// #define MANAGER_INITIAL_SHARKS_PER_SECOND (1.5)

// ALONB - this shouldn't be here
template <class T>
static T getRandomNumber(T min, T max) // ALONB - how could this class own some of the dimentions?
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

static_assert(GAME_MANAGER_COUNTDOWN_START_NUM <= 3);
static_assert(GAME_MANAGER_FLICKERS_WHEN_COLLIDE % 2 == 0);