#ifndef FAST_LED_SIMULATOR_H_
#define FAST_LED_SIMULATOR_H_

#include <iostream>
#include <SDL2/SDL.h>
#include <FastLED.h>

template<size_t size>
class FastLEDSimulator {
  public:
    // Initializes SDL. Call first.
    bool Init();

    // Runs the event loop. Call periodically. Returns false when the user closes the window.
    bool Run();

    // Closes the window. Call last.
    void Close();

    CRGB leds[size];

  protected:
    // Don't use this class directly - use a subclass.
    FastLEDSimulator(int led_circle_pixels, int led_square_pixels) : led_pixels_(led_circle_pixels), led_frame_pixels_(led_square_pixels) {}

    const int led_pixels_;
    const int led_frame_pixels_;

    SDL_Point led_locations_[size];

  private:
    SDL_Window* window_;
    SDL_Renderer *renderer_;

    void LogSDLError(const std::string component);
};

template<size_t size>
bool FastLEDSimulator<size>::Init() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    LogSDLError("SDL_Init: ");
    return false;
  }

  window_ = SDL_CreateWindow("FastLED Simulator", 100, 100, 2000, 100, SDL_WINDOW_SHOWN);
  if (window_ == nullptr){
    LogSDLError("SDL_CreateWindow: ");
    SDL_Quit();
    return false;
  }

  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer_ == nullptr){
    SDL_DestroyWindow(window_);
    LogSDLError("SDL_CreateRenderer: ");
    SDL_Quit();
    return false;
  }

  // Draw the frames around the LEDs
  SDL_SetRenderDrawColor(renderer_, 0x40, 0x40, 0x40, 0xFF);
  for (int i = 0; i < size; i++) {
    SDL_Rect rect = {led_locations_[i].x, led_locations_[i].y, led_frame_pixels_, led_frame_pixels_};
    SDL_RenderFillRect(renderer_, &rect);
  }
  SDL_RenderPresent(renderer_);

  return true;
}

template<size_t size>
bool FastLEDSimulator<size>::Run() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT || event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) {
      return false;
    }
  }

  for (int i = 0; i < size; i++) {
    int led_offset = (led_frame_pixels_ - led_pixels_) / 2;

    SDL_SetRenderDrawColor(renderer_, leds[i].r, leds[i].g, leds[i].b, 0xFF);
    SDL_Rect rect = {led_locations_[i].x + led_offset, led_locations_[i].y + led_offset, led_pixels_, led_pixels_};
    SDL_RenderFillRect(renderer_, &rect);
  }

  SDL_RenderPresent(renderer_);

  return true;
}

template<size_t size>
void FastLEDSimulator<size>::Close() {
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

template<size_t size>
void FastLEDSimulator<size>::LogSDLError(const std::string component) {
  std::cout << component << ": " << SDL_GetError() << std::endl;
}

#endif  // FAST_LED_SIMULATOR_H_
