#ifndef FAST_LED_SIMULATOR_H_
#define FAST_LED_SIMULATOR_H_

#include <iostream>
#include <SDL2/SDL.h>
#include <FastLED.h>

template<size_t size>
class FastLEDSimulator {
  public:
    FastLEDSimulator() {}

    // Initializes SDL. Call first.
    bool Init();

    // Runs the event loop. Call periodically. Returns false when the user closes the window.
    bool Run();

    // Closes the window. Call last.
    void Close();

  private:
    CRGB leds[size];
    SDL_Window* window;
    SDL_Renderer *renderer;

    void LogSDLError(const std::string component);
};

template<size_t size>
bool FastLEDSimulator<size>::Init() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    LogSDLError("SDL_Init: ");
    return false;
  }

  window = SDL_CreateWindow("FastLED Simulator", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
  if (window == nullptr){
    LogSDLError("SDL_CreateWindow: ");
    SDL_Quit();
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr){
    SDL_DestroyWindow(window);
    LogSDLError("SDL_CreateRenderer: ");
    SDL_Quit();
    return false;
  }

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

  SDL_Rect rect = {10, 10, 20, 20};
  SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
  SDL_RenderFillRect(renderer, &rect);

  SDL_RenderPresent(renderer);

  return true;
}

template<size_t size>
void FastLEDSimulator<size>::Close() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

template<size_t size>
void FastLEDSimulator<size>::LogSDLError(const std::string component) {
  std::cout << component << ": " << SDL_GetError() << std::endl;
}

#endif  // FAST_LED_SIMULATOR_H_
