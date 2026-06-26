cmake_minimum_required(VERSION 3.16)
project(Bagiroid)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(bagiroid ${SOURCES})

target_include_directories(bagiroid PRIVATE
    src
    src/game_objects
    src/plugin_api
    src/utility
    src/core)

target_link_libraries(bagiroid PRIVATE SDL3::SDL3)

# Compile example_plugin.cpp into a library and put it into plugins folder
add_library(example_plugin MODULE plugins/example/example_plugin.cpp)

target_include_directories(example_plugin PRIVATE src/plugin_api)

set_target_properties(example_plugin PROPERTIES
    PREFIX ""
    CXX_VISIBILITY_PRESET hidden
    LIBRARY_OUTPUT_DIRECTORY "$<TARGET_FILE_DIR:bagiroid>/plugins"
    RUNTIME_OUTPUT_DIRECTORY "$<TARGET_FILE_DIR:bagiroid>/plugins")