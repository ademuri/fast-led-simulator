// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "linear-simulator.h"

static constexpr int kSize = 30;

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
