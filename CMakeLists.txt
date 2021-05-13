cmake_minimum_required(VERSION 3.13)

project(laserkombat)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fpermissive")

set(SOURCES
    main.cpp
    Board.cpp
    Draw.cpp
    Exception.h
    Sound.cpp
    Square.h
    Tile.cpp
    constants.h
)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})