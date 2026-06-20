cmake_minimum_required(VERSION 3.16)
project(Bagiroid)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL3 REQUIRED)

file(GLOB SOURCES "src/*.cpp")
add_executable(bagiroid ${SOURCES})

target_link_libraries(bagiroid PRIVATE SDL3::SDL3)