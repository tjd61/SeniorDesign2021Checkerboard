#include "Buttons.h"
#include "Gameplay.h"
#include "LEDController.h"

struct Game game = {{0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,},0,0,0,0,0,0,0,0,0};
struct Game *pt = &game;

void setup() {
  setupLeds();
  SetBoard(pt);
  PrintBoard(pt);  //This is covered in the PrintBoard showLedArray(pt);
  
  //button_setup();
}

void loop() {
  //pressHandler();
  //showLedArray();
  //Play_Game(pt);
}
