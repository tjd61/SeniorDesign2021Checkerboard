#include "Buttons.h"
#include "Gameplay.h"

void setup() {
  //pixels.setBrightness(1);

  //pinMode(pinButtonTest, INPUT);

  button_setup();
  SetBoard();
  //showLedArray();
}

void loop() {
  //pressHandler();
  //showLedArray();
  Play_Game();
}
