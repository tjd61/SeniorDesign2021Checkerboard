#include "Buttons.h"
#include "Gameplay.h"

void setup() {
  //pixels.setBrightness(1);

  button_setup();
  SetBoard();
  //showLedArray();
}

void loop() {
  //pressHandler();
  //showLedArray();
  Play_Game();
}
