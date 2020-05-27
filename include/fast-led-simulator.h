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

  private:
    CRGB leds[size];

    void LogSDLError(const std::string component);
};

template<size_t size>
bool FastLEDSimulator<size>::Init() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    LogSDLError("SDL_Init: ");
    return false;
  }

  SDL_Window *win = SDL_CreateWindow("FastLED Simulator", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
  if (win == nullptr){
    LogSDLError("SDL_CreateWindow: ");
    SDL_Quit();
    return false;
  }

  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr){
    SDL_DestroyWindow(win);
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

  return true;
}

template<size_t size>
void FastLEDSimulator<size>::LogSDLError(const std::string component) {
  std::cout << component << ": " << SDL_GetError() << std::endl;
}

#endif  // FAST_LED_SIMULATOR_H_
