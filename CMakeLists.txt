cmake_minimum_required(VERSION 3.16)

project(laserkombat)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fpermissive")

file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
    if(MINGW)
        set(CMAKE_CXX_FLAGS "-static ${CMAKE_CXX_FLAGS}")
        target_link_libraries(${PROJECT_NAME} PRIVATE mingw32)
    endif()
else()
    add_executable(${PROJECT_NAME} ${SOURCES})
endif()

if(WIN32 OR PSP)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2main)
endif()

if(MINGW OR VITA)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2-static)
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})

if(VITA)
    include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)
    vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})

    vita_create_vpk(${PROJECT_NAME}.vpk "LKRE00001" ${PROJECT_NAME}.self
        VERSION "01.00"
        NAME "Laser Kombat"
        FILE platform/vita/sce_sys sce_sys
        FILE README.md README.md
        FILE COPYING COPYING
        FILE assets assets
    )
else()
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
endif()