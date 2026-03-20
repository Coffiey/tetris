cmake_minimum_required(VERSION 3.17)

PROJECT(minimax)
SET(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/external/SFML-3.0.2/include)
link_directories(${CMAKE_SOURCE_DIR}/external/SFML-3.0.2/lib)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION Release)
file(COPY ${CMAKE_SOURCE_DIR}/external/SFML-3.0.2/bin/ DESTINATION Release)

ADD_EXECUTABLE(tetris src/main.cpp)

target_link_libraries(tetris
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)