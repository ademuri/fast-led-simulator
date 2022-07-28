/*
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FAST_LED_SIMULATOR_H_
#define FAST_LED_SIMULATOR_H_

#if __APPLE__
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <FastLED.h>

#include <array>
#include <iostream>

struct Led {
  int led_size;
  int frame_size;
  SDL_Point location;
};

template <size_t size>
class FastLEDSimulator {
 public:
  // Initializes SDL. Call first.
  bool Init();

  // Runs the event loop. Call periodically. Returns false when the user closes
  // the window.
  bool Run();

  // Closes the window. Call last.
  void Close();

  CRGB leds[size];

 protected:
  // Don't use this class directly - use a subclass.
  FastLEDSimulator() {}

  SDL_Window *window_;
  SDL_Renderer *renderer_;

  std::array<Led, size> led_layout_;

  // Sets LED locations and sizes based on the current window size
  virtual void SetLedPositions() = 0;

  // Returns the initial size and position of the window
  virtual SDL_Point GetInitialSize() = 0;
  virtual SDL_Point GetInitialPosition() = 0;

  // Prints the current SDL error
  void LogSDLError(const std::string component);

  // Whether to enable the audio subsystem. Disabled by default.
  virtual bool EnableAudio();

  // Subclasses may use this to draw other things.
  virtual void DrawExtras() {}

 private:
  void DrawFrames();
  void DrawLeds();
};

template <size_t size>
bool FastLEDSimulator<size>::Init() {
  int flags = SDL_INIT_VIDEO;
  if (EnableAudio()) {
    flags |= SDL_INIT_AUDIO;
  }
  if (SDL_Init(flags) != 0) {
    LogSDLError("SDL_Init: ");
    return false;
  }

  SDL_Point initial_size = GetInitialSize();
  SDL_Point initial_position = GetInitialPosition();
  window_ = SDL_CreateWindow("FastLED Simulator", initial_position.x,
                             initial_position.y, initial_size.x, initial_size.y,
                             SDL_WINDOW_RESIZABLE);
  if (window_ == nullptr) {
    LogSDLError("SDL_CreateWindow");
    SDL_Quit();
    return false;
  }

  renderer_ = SDL_CreateRenderer(
      window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer_ == nullptr) {
    SDL_DestroyWindow(window_);
    LogSDLError("SDL_CreateRenderer");
    SDL_Quit();
    return false;
  }

  SetLedPositions();
  DrawFrames();
  DrawExtras();
  SDL_RenderPresent(renderer_);

  return true;
}

template <size_t size>
bool FastLEDSimulator<size>::Run() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        return false;

      case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          return false;
        }
        break;

      case SDL_WINDOWEVENT:
        switch (event.window.event) {
          case SDL_WINDOWEVENT_RESIZED:
          case SDL_WINDOWEVENT_SIZE_CHANGED:
            SetLedPositions();
            break;
        }
    }
  }

  DrawFrames();
  DrawLeds();
  DrawExtras();
  SDL_RenderPresent(renderer_);

  return true;
}

template <size_t size>
void FastLEDSimulator<size>::Close() {
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

template <size_t size>
void FastLEDSimulator<size>::DrawFrames() {
  SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0xFF);
  SDL_RenderClear(renderer_);

  SDL_SetRenderDrawColor(renderer_, 0x40, 0x40, 0x40, 0xFF);
  for (size_t i = 0; i < size; i++) {
    SDL_Rect rect = {led_layout_[i].location.x, led_layout_[i].location.y,
                     led_layout_[i].frame_size, led_layout_[i].frame_size};
    SDL_RenderFillRect(renderer_, &rect);
  }
}

template <size_t size>
void FastLEDSimulator<size>::DrawLeds() {
  for (size_t i = 0; i < size; i++) {
    int led_offset = (led_layout_[i].frame_size - led_layout_[i].led_size) / 2;

    SDL_SetRenderDrawColor(renderer_, leds[i].r, leds[i].g, leds[i].b, 0xFF);
    SDL_Rect rect = {led_layout_[i].location.x + led_offset,
                     led_layout_[i].location.y + led_offset,
                     led_layout_[i].led_size, led_layout_[i].led_size};
    SDL_RenderFillRect(renderer_, &rect);
  }
}

template <size_t size>
void FastLEDSimulator<size>::LogSDLError(const std::string component) {
  std::cout << component << ": " << SDL_GetError() << std::endl;
}

template <size_t size>
bool FastLEDSimulator<size>::EnableAudio() {
  return false;
}

#endif  // FAST_LED_SIMULATOR_H_
