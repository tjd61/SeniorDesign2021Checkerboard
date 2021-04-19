#include "Arduino.h"
#include "Buttons.h"
#include "LEDController.h"

void button_setup(void) {
  for (int pin : buttonTestRows) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  for (int pin : buttonTestCols) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  pinMode(pinButtonTest, INPUT);
  pinMode(Pin23, OUTPUT);
}

void keystroke(int row, int col, int coordNum, struct Game *p) {
  //Serial.print("Pressed r:");
  //Serial.print(row);
  //Serial.print(" c:");
  //Serial.println(col);
  /*for(int i = 0; i < 16; i++) {
    pixels.setPixel(i, 0,0,0);
  }*/
  
  //row is the row of the button pressed from 0 to 7 representing rows 1 to 8 respectively
  //col is the col of the button pressed from 0 to 7 representing rows A to H respectively
  
  //Integration of rule checking code here. Code is allowed to change p.board.
  
  //Test code to turn off led at pressed position
  //p->board[row][col] = 0;
  
  //if coords == 1 then load input to x0 and y0 like above
  //if coords == 2 then load input to x1 and y1 like above
  if(coordNum == 1){
    p->x0 = col;
    p->y0 = row;
  }else if(coordNum == 2){
    p->x1 = col;
    p->y1 = row;
  }
  //showLedArray();

  
  /*if(row%2) {
    col = 7 - col;
  }
  pixels.setPixel(row*8 + col, 255, 255, 255);*/
  
}


int pressHandler(int coordNum, struct Game *p) {
  if(true) {
    //Serial.println("\n Running Press Handler");
    //count = 0;
    
    /*The following code is meant to be used to 
      sweep the created button array through binary
    */
    
    int pinR2 = buttonTestRows[0];
    int pinR1 = buttonTestRows[1];
    int pinR0 = buttonTestRows[2];
    int pinC2 = buttonTestCols[0];
    int pinC1 = buttonTestCols[1];
    int pinC0 = buttonTestCols[2];

    int row = 0;
    for(int i = 0; i < 8; i++) {
      //pinR2 is most significant
      //pinR0 is least significant
      switch(i){
        case 0 :
          digitalWrite(pinR2, LOW);
          digitalWrite(pinR1, LOW);
          digitalWrite(pinR0, LOW);
          break;
        case 1 :
          digitalWrite(pinR2, LOW);
          digitalWrite(pinR1, LOW);
          digitalWrite(pinR0, HIGH);
          break;
        case 2 :
          digitalWrite(pinR2, LOW);
          digitalWrite(pinR1, HIGH);
          digitalWrite(pinR0, LOW);
          break;
        case 3 :
          digitalWrite(pinR2, LOW);
          digitalWrite(pinR1, HIGH);
          digitalWrite(pinR0, HIGH);
          break;
        case 4 :
          digitalWrite(pinR2, HIGH);
          digitalWrite(pinR1, LOW);
          digitalWrite(pinR0, LOW);
          break;
        case 5 :
          digitalWrite(pinR2, HIGH);
          digitalWrite(pinR1, LOW);
          digitalWrite(pinR0, HIGH);
          break;
        case 6 :
          digitalWrite(pinR2, HIGH);
          digitalWrite(pinR1, HIGH);
          digitalWrite(pinR0, LOW);
          break;
        case 7 :
          digitalWrite(pinR2, HIGH);
          digitalWrite(pinR1, HIGH);
          digitalWrite(pinR0, HIGH);
          break;
      }

      int col = 0;
      for(int j = 0; j < 8; j++) {
        //pinR2 is most significant
        //pinR0 is least significant
        switch(j){
          case 0 :
            digitalWrite(pinC2, LOW);
            digitalWrite(pinC1, LOW);
            digitalWrite(pinC0, LOW);
            break;
          case 1 :
            digitalWrite(pinC2, LOW);
            digitalWrite(pinC1, LOW);
            digitalWrite(pinC0, HIGH);
            break;
          case 2 :
            digitalWrite(pinC2, LOW);
            digitalWrite(pinC1, HIGH);
            digitalWrite(pinC0, LOW);
            break;
          case 3 :
            digitalWrite(pinC2, LOW);
            digitalWrite(pinC1, HIGH);
            digitalWrite(pinC0, HIGH);
            break;
          case 4 :
            digitalWrite(pinC2, HIGH);
            digitalWrite(pinC1, LOW);
            digitalWrite(pinC0, LOW);
            break;
          case 5 :
            digitalWrite(pinC2, HIGH);
            digitalWrite(pinC1, LOW);
            digitalWrite(pinC0, HIGH);
            break;
          case 6 :
            digitalWrite(pinC2, HIGH);
            digitalWrite(pinC1, HIGH);
            digitalWrite(pinC0, LOW);
            break;
          case 7 :
            digitalWrite(pinC2, HIGH);
            digitalWrite(pinC1, HIGH);
            digitalWrite(pinC0, HIGH);
            break;
        }
        if(digitalRead(pinButtonTest) == LOW){
          Serial.println("\n Found Button Press");
          keystroke(row, col, coordNum, p);
          //Delay after a button is pressed
          int startTime = millis();
          int stopTime = millis();
          //Change 500 to the time in milliseconds for the delay between sequential button presses
          while((stopTime - startTime) < 500){
            stopTime = millis();
          }
          return 1;
        }
        col++;
      }
      row++;
    }
  }
  return 0;
}
