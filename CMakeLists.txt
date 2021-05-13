cmake_minimum_required(VERSION 3.13)

project(laserkombat)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(SOURCES
    Board.cpp
    ddutil.cpp
    Draw.cpp
    Exception.h
    pix.cpp
    sound.cpp
    Square.h
    Tile.cpp
    UseDdraw.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARY})