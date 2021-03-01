#include "NDPixel.h"

#define R1 3
#define R2 2
#define R3 1
#define R4 0

#define C1 10
#define C2 16
#define C3 14
#define C4 15

int rows[4] = {R1, R2, R3, R4};
int cols[4] = {C1, C2, C3, C4};
volatile boolean pressed[4][4];

volatile static uint32_t threshold = 50000;
volatile static uint32_t count = 0;

NDPixel pixels(8, 16);

void setup() {
  OCR0A = 0xAF; //Set Timer 0 Counter Register Value
  TIMSK0 |= _BV(OCIE0A);

  pixels.setBrightness(100);
  
  for (int pin : rows) {
    pinMode(pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(pin), pressHandler, HIGH);
  }
  for (int pin : cols) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
}

//INTERRUPT HANDLING
SIGNAL(TIMER0_COMPA_vect) { //Timer 0 Counter Comparison 
  pixels.updateLEDs();
}

void loop() {
  noInterrupts();
  if(count < threshold) {
    count++;
    //Serial.println(count);
  }
  else if(count == threshold) {
    //Serial.println("Ready!");
    count++;
  }
  interrupts();
}


void keystroke(int row, int col) {
  //Serial.print("Pressed r:");
  //Serial.print(row);
  //Serial.print(" c:");
  //Serial.println(col);
  for(int i = 0; i < 16; i++) {
    pixels.setPixel(i, 0,0,0);
  }
  if(row%2) {
    col = 3 - col;
  }
  pixels.setPixel(row*4 + col, 255, 255, 255);
  
}


void pressHandler() {
  if(count > threshold) {
    count = 0;
    //Serial.println("Press detected!");
  
    int row = 0;
    for(int pin : rows) {
      if(digitalRead(pin) == HIGH) {
        
        int col = 0;
        for(int pin2 : cols) {
          digitalWrite(pin2, LOW);
          if (digitalRead(pin) == LOW) {
            keystroke(row, col);
          }
          digitalWrite(pin2, HIGH);
          col++;
        }
      }
      row++;
    }
  }
}
