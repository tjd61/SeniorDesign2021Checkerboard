#include "Arduino.h"
#include "HM10.h"
#include <SoftwareSerial.h>

SoftwareSerial BLE_SERIAL(RXPIN, TXPIN);

void bleInitialize() {
  BLE_SERIAL.begin(9600);
  delay(2000);
//  BLE_SERIAL.println("AT+VERSION");
//  delay(20);
//  while(BLE_SERIAL.available()) {
//    Serial.write(BLE_SERIAL.read());
//  }
}

void bleWrite(String charString) {
  char buffer[120];
  charString.toCharArray(buffer, charString.length());
  BLE_SERIAL.write(buffer);
}

String bleRead(void) {
  if(!BLE_SERIAL.available()) return "";

  String buffer;
  buffer.reserve(120);
  while(BLE_SERIAL.available()) {
    buffer.concat(BLE_SERIAL.readStringUntil('\r\n'));
    delay(2);
  }
  return buffer;
}

void bleDebug(void) {
  while(Serial.available()) {
    BLE_SERIAL.write(Serial.read());
  }
}

int bluetoothCommandParser(int coordNum, struct Game *p) {
  String bleData = bleRead();
  if(bleData != "") {
    if(bleData.length() < 2) return 0;
    bleData.toUpperCase();
    uint8_t col = int(bleData.charAt(0)) - 48 - 17;
    uint8_t row = int(bleData.charAt(1)) - 48 - 1;
    
    if(coordNum == 1){
      p->x0 = col;
      p->y0 = row;
    }
    else if(coordNum == 2){
      p->x1 = col;
      p->y1 = row;
    }
    return 1;
  }
  return 0;
}
