#!/bin/bash
# Copyright 2020 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.11)
project(fast-led-simulator CXX)
set(CMAKE_CXX_STANDARD 11)

# Generate the file needed for YCM integration
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

include(FetchContent)
FetchContent_Declare(fake-fast-led
    GIT_REPOSITORY      https://github.com/ademuri/FakeFastLED.git
    GIT_TAG             master
)
if (${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.14)
  FetchContent_MakeAvailable(fake-fast-led)
else()
  FetchContent_GetProperties(fake-fast-led)
  if(NOT fake-fast-led_POPULATED)
    FetchContent_Populate(fake-fast-led)
    add_subdirectory(${fake-fast-led_SOURCE_DIR} ${fake-fast-led_BINARY_DIR})
  endif()
endif()

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
  SDL2
  fake-fast-led)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Build examples
  add_subdirectory(examples)

endif()

