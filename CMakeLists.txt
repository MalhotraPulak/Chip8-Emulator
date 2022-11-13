cmake_minimum_required(VERSION 3.19)
project(Chip8)

set(CMAKE_CXX_STANDARD 20)
find_package(curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
file(GLOB SOURCES "*.c")
add_executable(Chip8 main.cpp C8.cpp C8.h)
target_link_libraries(Chip8 ${CURSES_LIBRARIES})