cmake_minimum_required(VERSION 2.6)

project(FastLEDSimulator CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_party/sdl2-cmake-modules)

find_package(SDL2 REQUIRED)

#target_link_libraries(${PROJECT_NAME} SDL2::Main)

