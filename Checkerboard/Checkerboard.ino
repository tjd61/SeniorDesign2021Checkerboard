#include "Buttons.h"
#include "Gameplay.h"

void setup() {
  //pixels.setBrightness(1);

  //pinMode(pinButtonTest, INPUT);

  button_setup();
  //showLedArray();
  SetBoard();
}

void loop() {
  //The following code isn't necessary for the button or LED code
  //pressHandler();
  //showLedArray();
  Play_Game();
}
