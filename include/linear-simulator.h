#ifndef LINEAR_SIMULATOR_H_
#define LINEAR_SIMULATOR_H_

#include "fast-led-simulator.h"

template<size_t size>
class LinearSimulator : public FastLEDSimulator<size> {
  public:
    LinearSimulator();

  private:
    static const int kPixelMargin = 10;
};

template<size_t size>
LinearSimulator<size>::LinearSimulator() : FastLEDSimulator<size>(36, 48) {
  for (int i = 0; i < size; i++) {
    this->leds[i] = CRGB(0, 0, 0);
    this->led_locations_[i] = {kPixelMargin + (this->led_frame_pixels_ + kPixelMargin) * i, kPixelMargin};
  }
}

#endif  // LINEAR_SIMULATOR_H_
