#include "HM10.h"

void setup() {
  Serial.begin(9600);
  bleInitialize();
}


void loop() {
  String bleData = bleRead();
  if(bleData != "") {
    Serial.println(bleData);
    //bleWrite("Hi!");
    bluetoothCommandParser(bleData);
  }

  //bleDebug();
}

void bluetoothCommandParser(String data) {
  if(data.length() != 2) return;
  data.toUpperCase();
  uint8_t col = int(data.charAt(0)) - 48 - 17;
  uint8_t row = int(data.charAt(1)) - 48;
  Serial.print("row: ");
  Serial.print(row);
  Serial.print("\tcol: ");
  Serial.println(col);
}
