cmake_minimum_required(VERSION 3.10)
project(MyCMakeProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)  # Ensure debug info is generated
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")# // adjust as necessary

set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/mylib.cpp
    src/main.cpp
)

# Create the executable
add_executable(MyCMakeProject ${SOURCES})