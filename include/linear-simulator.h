#ifndef LINEAR_SIMULATOR_H_
#define LINEAR_SIMULATOR_H_

#include "fast-led-simulator.h"

template<size_t size>
class LinearSimulator : public FastLEDSimulator<size> {
  public:
    LinearSimulator();

  protected:
    void SetLedPositions() override;

    SDL_Point GetInitialSize() override;
    SDL_Point GetInitialPosition() override;

  private:
    static const int kLedMarginPixels = 4;

    int CalculateFrameWidth(int screen_width);
};

template<size_t size>
LinearSimulator<size>::LinearSimulator() : FastLEDSimulator<size>() {}

template<size_t size>
void LinearSimulator<size>::SetLedPositions() {
  int width = 1024;
  if (SDL_GetRendererOutputSize(this->renderer_, &width, nullptr)) {
    this->LogSDLError("SDL_GetRendererOutputSize");
  }

  this->led_frame_pixels_ = CalculateFrameWidth(width);
  this->led_pixels_ = this->led_frame_pixels_ - 4;
  for (int i = 0; i < size; i++) {
    this->leds[i] = CRGB(0, 0, 0);
    this->led_locations_[i] = {kLedMarginPixels + (this->led_frame_pixels_ + kLedMarginPixels) * i, kLedMarginPixels};
  }
}

template<size_t size>
SDL_Point LinearSimulator<size>::GetInitialSize() {
  // Make the window as wide as possible. Note: this doesn't work with display scaling!
  int width = 1024;
  SDL_DisplayMode display_mode;
  if (SDL_GetCurrentDisplayMode(0, &display_mode)) {
    this->LogSDLError("SDL_GetDisplayUsableBounds");
  } else {
    width = display_mode.w;
  }

  return {width, CalculateFrameWidth(width) + kLedMarginPixels * 2};
}

template<size_t size>
SDL_Point LinearSimulator<size>::GetInitialPosition() {
  return {0, 100};
}

template<size_t size>
int LinearSimulator<size>::CalculateFrameWidth(int screen_width) {
  return (1.0 * screen_width - this->kLedMarginPixels * (size + 1)) / size;
}

#endif  // LINEAR_SIMULATOR_H_
