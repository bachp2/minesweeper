cmake_minimum_required(VERSION 3.8)
project(minesweeper)

set(CMAKE_CXX_STANDARD 11)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(SOURCE_FILES main.cpp game.cpp game.h)
add_executable(minesweeper ${SOURCE_FILES})
target_link_libraries(minesweeper ${CURSES_LIBRARIES})
