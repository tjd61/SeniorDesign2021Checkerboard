#include "HM10.h"

void setup() {
  Serial.begin(9600);
  bleInitialize();
}


void loop() {
  String bleData = bleRead();
  if(bleData != "") {
    Serial.print(bleData);
    bleWrite("Hi!");
  }

  bleDebug();
}
