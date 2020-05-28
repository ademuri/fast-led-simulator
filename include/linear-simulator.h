#ifndef LINEAR_SIMULATOR_H_
#define LINEAR_SIMULATOR_H_

#include "fast-led-simulator.h"

template<size_t size>
class LinearSimulator : public FastLEDSimulator<size> {
  public:
    LinearSimulator();

  protected:
    void SetLedPositions() override;

  private:
    static const int kLedMarginPixels = 4;
};

template<size_t size>
LinearSimulator<size>::LinearSimulator() : FastLEDSimulator<size>() {}

template<size_t size>
void LinearSimulator<size>::SetLedPositions() {
  int width = 1024;
  if (SDL_GetRendererOutputSize(this->renderer_, &width, nullptr)) {
    this->LogSDLError("SDL_GetRendererOutputSize");
  }

  this->led_frame_pixels_ = (1.0 * width - this->kLedMarginPixels * (size + 1)) / size;
  this->led_pixels_ = this->led_frame_pixels_ - 4;
  for (int i = 0; i < size; i++) {
    this->leds[i] = CRGB(0, 0, 0);
    this->led_locations_[i] = {kLedMarginPixels + (this->led_frame_pixels_ + kLedMarginPixels) * i, kLedMarginPixels};
  }
}

#endif  // LINEAR_SIMULATOR_H_
