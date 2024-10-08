cmake_minimum_required(VERSION 3.10)
project(MyCMakeProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)  # Ensure debug info is generated
message(STATUS "Using directory: $ENV{SFML_DIR}")
set(SFML_DIR "$ENV{SFML_DIR}\\lib\\cmake\\SFML" CACHE STRING "Path to SFML installation")

find_package(SFML REQUIRED COMPONENTS graphics window system)

# Include directories
include_directories(MyCMakeProject include)
include_directories(MyCMakeProject $ENV{SFML_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/manager.cpp
    src/screen.cpp
    src/gameShapes.cpp
    src/objects.cpp
    src/dimensions.cpp
)

link_directories($ENV{SFML_DIR}/lib)

# Create the executable
add_executable(MyCMakeProject ${SOURCES})

# Add compiler flags
target_compile_options(MyCMakeProject PRIVATE
    -Werror              # Treat warnings as errors
    -Wunused-variable     # Warn about unused variables
    -Wunused-function     # Warn about unused functions
    # -Wunused-parameter          # Warn about unused parameters in functions ALONB - add this!

)

if(CMAKE_BUILD_TYPE MATCHES Debug)
set(SFML_LIBRARIES sfml-graphics-d sfml-window-d sfml-system-d)
else()
set(SFML_LIBRARIES sfml-graphics sfml-window sfml-system)
endif()
target_link_libraries(MyCMakeProject ${SFML_LIBRARIES})
