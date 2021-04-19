#include "Buttons.h"
#include "Gameplay.h"
#include "LEDController.h"
#include "HM10.h"
#include <Wire.h>

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
  //bleInitialize();
  SetBoard(pt);
  PrintBoard(pt);  //This is covered in the PrintBoard showLedArray(pt);
  
  button_setup();
  Wire.begin();        //join i2c bus
  Serial.begin(9600);
}

void loop() {
  //pressHandler();
  //showLedArray();
  Play_Game(pt);
}
