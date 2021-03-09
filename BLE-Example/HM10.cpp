#include "Arduino.h"
#include "HM10.h"
#include <SoftwareSerial.h>

SoftwareSerial BLE_SERIAL(RXPIN, TXPIN);

void bleInitialize() {
  BLE_SERIAL.begin(9600);
  
  BLE_SERIAL.println("AT+VERSION");
  delay(20);
  while(BLE_SERIAL.available()) {
    Serial.write(BLE_SERIAL.read());
  }
}

void bleName(String newName) {
  
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
    buffer += char(BLE_SERIAL.read());
    delay(20);
  }
  return buffer;
}

void bleDebug(void) {
  while(Serial.available()) {
    BLE_SERIAL.write(Serial.read());
  }
}
