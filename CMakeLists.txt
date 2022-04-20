cmake_minimum_required(VERSION 3.16)

project(laserkombat)

find_package(SDL2 REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fpermissive")

file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
    if(MINGW)
        set(CMAKE_CXX_FLAGS "-static ${CMAKE_CXX_FLAGS}")
        target_link_libraries(${PROJECT_NAME} PRIVATE mingw32 SDL2main SDL2::SDL2-static)
    endif()
else()
    add_executable(${PROJECT_NAME} ${SOURCES})
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
