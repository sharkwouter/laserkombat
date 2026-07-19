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
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/platform/windows/appicon.rc)
else()
    add_executable(${PROJECT_NAME} ${SOURCES})
endif()

# Link SDL2main for systems which need it
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2main)
endif()

# Set the build static option
SET(BUILD_STATIC_DEFAULT OFF)
if(WIN32 OR VITA OR PSP)
    SET(BUILD_STATIC_DEFAULT ON)
endif()
option(BUILD_STATIC "Build a static binary" ${BUILD_STATIC_DEFAULT})

if(BUILD_STATIC)
    set(CMAKE_CXX_FLAGS "-static ${CMAKE_CXX_FLAGS}")
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_STATIC_LIBRARIES} ${SDL2_TTF_STATIC_LIBRARIES})
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

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
elseif(PSP)
    create_pbp_file(
        TARGET ${PROJECT_NAME}
        ICON_PATH NULL
        BACKGROUND_PATH NULL
        PREVIEW_PATH NULL
        TITLE "Laser Kombat"
        VERSION 01.00
    )
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/fonts ${CMAKE_CURRENT_BINARY_DIR}/assets/fonts)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/images16x16 ${CMAKE_CURRENT_BINARY_DIR}/assets/images)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/levels ${CMAKE_CURRENT_BINARY_DIR}/assets/levels)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/sounds ${CMAKE_CURRENT_BINARY_DIR}/assets/sounds)
else()
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
endif()
