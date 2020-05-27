#include "fast-led-simulator.h"

int main() {
  FastLEDSimulator<30> simulator;
  simulator.Init();

  while (simulator.Run());

  simulator.Close();
}
