#include "Buttons.h"
#include "Gameplay.h"

struct Game game = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0,0,0,0,0,0};
struct Game *pt = &game;

void setup() {
  //pixels.setBrightness(1);

  button_setup();
  SetBoard(pt);
  //showLedArray();
}

void loop() {
  //pressHandler();
  //showLedArray();
  Play_Game(pt);
}
