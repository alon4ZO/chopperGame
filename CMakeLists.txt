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

if(CMAKE_BUILD_TYPE MATCHES Debug)
set(SFML_LIBRARIES sfml-graphics-d sfml-window-d sfml-system-d)
else()
set(SFML_LIBRARIES sfml-graphics sfml-window sfml-system)
endif()
target_link_libraries(MyCMakeProject ${SFML_LIBRARIES})





# Set the source directory from environment variable or default
# if(DEFINED ENV{IMAGE_SOURCE_DIR})
#     set(IMAGE_SOURCE_DIR $ENV{IMAGE_SOURCE_DIR})
# else()
#     message(FATAL_ERROR "Environment variable IMAGE_SOURCE_DIR not defined.")
# endif()

# # Check if the IMAGE_SOURCE_DIR exists
# if(NOT EXISTS ${IMAGE_SOURCE_DIR} OR NOT IS_DIRECTORY ${IMAGE_SOURCE_DIR})
#     message(FATAL_ERROR "The specified IMAGE_SOURCE_DIR does not exist: ${IMAGE_SOURCE_DIR}")
# endif()

# # Set the target assets directory
# set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")

# # Ensure that the assets directory exists
# file(MAKE_DIRECTORY ${ASSETS_DIR})


# function(copy_if_needed src_file dest_file)
#     if(NOT EXISTS ${dest_file})
#         file(COPY ${src_file} DESTINATION ${dest_file})
#     endif()
# endfunction()


# file(GLOB IMAGE_FILES "${IMAGE_SOURCE_DIR}/*.png")

# # Create a custom command that will execute the copying process
# foreach(IMAGE_FILE ${IMAGE_FILES})
#     get_filename_component(FILE_NAME ${IMAGE_FILE} NAME)
#     set(DEST_FILE "${ASSETS_DIR}/${FILE_NAME}")

#     add_custom_command(
#         OUTPUT ${DEST_FILE}
#         COMMAND ${CMAKE_COMMAND} -E copy_if_needed ${IMAGE_FILE} ${DEST_FILE}
#         DEPENDS ${IMAGE_FILE}
#         COMMENT "Copying ${IMAGE_FILE} to ${ASSETS_DIR}"
#     )
# endforeach()

# # Create a custom target that depends on all copied image files
# set(IMAGE_COPY_TARGET_NAME "CopyImages")
# add_custom_target(${IMAGE_COPY_TARGET_NAME} ALL DEPENDS ${IMAGE_FILES})