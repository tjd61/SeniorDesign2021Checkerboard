//Last modified 3/10/21 at 4:00 PM by Dalton Hill

#include "NDPixel.h"

#define BR2 2
#define BR1 0
#define BR0 1
#define BC2 10
#define BC1 16
#define BC0 14

#define pinButtonTest 5

//Most to least significant bit
//List of pins to be used for sweeping button rows
int buttonTestRows[3] = {BR2, BR1, BR0};
//List of pins to be used for sweeping button columns
int buttonTestCols[3] = {BC2, BC1, BC0};

int white[3]   = {255, 255, 255};
int black[3]   = {0,   0,   0};
int blue[3]    = {0,   0,   255};
int red[3]     = {255, 0,   0};
int cyan[3]    = {100, 255, 255};
int green[3]   = {0,   255, 0};
int yellow[3]  = {255, 255, 0};
int lblue[3]   = {0,   0,   128};
int lred[3]   = {128,   0,  0};


//1 for player 1 piece
//2 for player 2 piece
//3 for player 1 king
//4 for player 2 king

/*int ledColor[16] = {1, 2, 0, 0, 
                    0, 1, 2, 0, 
                    0, 0, 1, 2, 
                    0, 0, 0, 1,};*/

int ledColor[8][8] = {{1, 1, 1, 1, 2, 2, 2, 2,},
                      {3, 3, 3, 3, 4, 4, 4, 4,},
                      {1, 1, 1, 1, 2, 2, 2, 2,},
                      {3, 3, 3, 3, 4, 4, 4, 4,},
                      {1, 1, 1, 1, 2, 2, 2, 2,},
                      {3, 3, 3, 3, 4, 4, 4, 4,},
                      {1, 1, 1, 1, 2, 2, 2, 2,},
                      {3, 3, 3, 3, 4, 4, 4, 4,},};

int arraySize = 4;
                      
volatile boolean pressed[4][4];

volatile static uint32_t threshold = 50000;
volatile static uint32_t count = 0;

NDPixel pixels(8, 24);

void setup() {
  OCR0A = 0xAF; //Set Timer 0 Counter Register Value
  TIMSK0 |= _BV(OCIE0A);

  pixels.setBrightness(1);

  pinMode(pinButtonTest, INPUT);
  //attachInterrupt(digitalPinToInterrupt(pinButtonTest), pressHandler, LOW);

  for (int pin : buttonTestRows) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  for (int pin : buttonTestCols) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  
  showLedArray();
}

//INTERRUPT HANDLING
SIGNAL(TIMER0_COMPA_vect) { //Timer 0 Counter Comparison 
  pixels.updateLEDs();
  pressHandler();
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


void showLedArray() {
  for(int showRow = 0; showRow < 8; showRow++){
    for(int testCol = 0; testCol < 8; testCol++){
      int showCol;
      if(showRow%2) {
        showCol = 7 - testCol;
      } else {
        showCol = testCol;
      }
      
      int color[3];
      //No piece
      if(ledColor[showRow][showCol] == 0){
        for(int i = 0; i < 3; i++){
          color[i] = {black[i]};
        }
      //Player 1 Piece
      }else if(ledColor[showRow][showCol] == 1){
        for(int i = 0; i < 3; i++){
          color[i] = {blue[i]};
        }
      //Player 2 Piece
      }else if(ledColor[showRow][showCol] == 2){
        for(int i = 0; i < 3; i++){
          color[i] = {red[i]};
        }
      //Player 1 King
      }else if(ledColor[showRow][showCol] == 3){
        for(int i = 0; i < 3; i++){
          color[i] = {green[i]};
        }
      //Player 2 King
      }else if(ledColor[showRow][showCol] == 4){
        for(int i = 0; i < 3; i++){
          color[i] = {yellow[i]};
        }
      }
      pixels.setPixel(showRow*8 + testCol, color[0], color[1], color[2]);
      /*
      Serial.println("Position");
      Serial.println(showRow);
      Serial.println(testCol);
      Serial.println("Color");
      Serial.println(color[0]);
      Serial.println(color[1]);
      Serial.println(color[2]);
      Serial.println("  ");*/
    }
  }
}

void keystroke(int row, int col) {
  Serial.print("Pressed r:");
  Serial.print(row);
  Serial.print(" c:");
  Serial.println(col);
  /*for(int i = 0; i < 16; i++) {
    pixels.setPixel(i, 0,0,0);
  }*/

  
  //Checkerboard code
  int firstPressedx = 0;
  int firstPressedy = 0;
  
  ledColor[row][col] = 0;
  
  showLedArray();

  
  /*if(row%2) {
    col = 7 - col;
  }
  pixels.setPixel(row*8 + col, 255, 255, 255);*/
  
}


void pressHandler() {
  if(count > threshold) {
    count = 0;
    
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
        delay(1000);
        if(digitalRead(pinButtonTest) == LOW){
          keystroke(row,col);
          //delay(1000);
        }
        col++;
      }
      row++;
    }
  }
}
