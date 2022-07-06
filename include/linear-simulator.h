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

#ifndef LINEAR_SIMULATOR_H_
#define LINEAR_SIMULATOR_H_

#include "fast-led-simulator.h"

template <size_t size>
class LinearSimulator : public FastLEDSimulator<size> {
 public:
  LinearSimulator();

 protected:
  void SetLedPositions() override;

  SDL_Point GetInitialSize() override;
  SDL_Point GetInitialPosition() override;

  LedSize GetLedSize() override;

  int CalculateFrameWidth(int screen_width);

  static const int kLedMarginPixels = 4;
};

template <size_t size>
LinearSimulator<size>::LinearSimulator() : FastLEDSimulator<size>() {}

template <size_t size>
void LinearSimulator<size>::SetLedPositions() {
  const LedSize led_size = GetLedSize();

  for (size_t i = 0; i < size; i++) {
    this->leds[i] = CRGB(0, 0, 0);
    this->led_locations_[i] = {
        (int)(kLedMarginPixels + (led_size.frame_size + kLedMarginPixels) * i),
        kLedMarginPixels};
  }
}

template <size_t size>
SDL_Point LinearSimulator<size>::GetInitialSize() {
  // Make the window as wide as possible. Note: this doesn't work with display
  // scaling!
  int width = 1024;
  SDL_DisplayMode display_mode;
  if (SDL_GetCurrentDisplayMode(0, &display_mode)) {
    this->LogSDLError("SDL_GetDisplayUsableBounds");
  } else {
    width = display_mode.w;
  }

  return {width, CalculateFrameWidth(width) + kLedMarginPixels * 2};
}

template <size_t size>
SDL_Point LinearSimulator<size>::GetInitialPosition() {
  return {0, 100};
}

template <size_t size>
LedSize LinearSimulator<size>::GetLedSize() {
  int width = 1024;
  if (SDL_GetRendererOutputSize(this->renderer_, &width, nullptr)) {
    this->LogSDLError("SDL_GetRendererOutputSize");
  }

  LedSize ret;
  ret.frame_size = CalculateFrameWidth(width);
  ret.led_size = ret.frame_size - 4;
  return ret;
}

template <size_t size>
int LinearSimulator<size>::CalculateFrameWidth(int screen_width) {
  return (1.0 * screen_width - this->kLedMarginPixels * (size + 1)) / size;
}

#endif  // LINEAR_SIMULATOR_H_
