cmake_minimum_required(VERSION 2.6)

# TODO: can this have the same name as the lib?
project(fast-led-simulator CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_party/sdl2-cmake-modules)

find_package(SDL2 REQUIRED)

add_library(fast-led-simulator
 src/fast-led-simulator.cc) 

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(fast-led-simulator PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

target_link_libraries(fast-led-simulator
  SDL2)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Build examples
  add_subdirectory(examples)

endif()

