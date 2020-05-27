#include "linear-simulator.h"

const int kSize = 30;

int main() {
  LinearSimulator<kSize> simulator;
  simulator.Init();

  int counter = 0;
  while (simulator.Run()) {
    for (int i = 0; i < kSize; i++) {
      simulator.leds[i] = CHSV(counter + i * 10, 255, 255);
    }

    counter++;
  }

  simulator.Close();
}
