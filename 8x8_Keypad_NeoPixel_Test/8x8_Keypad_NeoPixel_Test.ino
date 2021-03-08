//Last modified 3/1/21 at 10:00 PM by Dalton Hill

#include "NDPixel.h"

#define R1 3
#define R2 2
#define R3 1
#define R4 0

#define C1 10
#define C2 16
#define C3 14
#define C4 15

int buttonRows[4] = {R1, R2, R3, R4};
int buttonCols[4] = {C1, C2, C3, C4};

int white[3]   = {255, 255, 255};
int black[3]   = {0,   0,   0};
int blue[3]    = {0,   0,   255};
int red[3]     = {255, 0,   0};
int yellow[3]  = {0,   255, 255};
int green[3]   = {0,   255, 0};


//1 for player 1 piece
//2 for player 2 piece
//3 for player 1 king
//4 for player 2 king

/*int ledColor[16] = {1, 2, 0, 0, 
                    0, 1, 2, 0, 
                    0, 0, 1, 2, 
                    0, 0, 0, 1,};*/

int ledColor[8][8] = {{1, 2, 0, 0, 0, 2, 0, 0,},
                      {0, 1, 2, 0, 0, 0, 0, 0,},
                      {0, 0, 1, 2, 0, 0, 0, 0,},
                      {0, 0, 0, 1, 0, 0, 0, 0,},
                      {0, 0, 0, 0, 0, 0, 0, 0,},
                      {0, 0, 0, 0, 0, 0, 0, 0,},
                      {0, 0, 0, 0, 0, 0, 0, 0,},
                      {0, 0, 0, 0, 0, 0, 0, 0,}};

int arraySize = 4;
                      
volatile boolean pressed[4][4];

volatile static uint32_t threshold = 50000;
volatile static uint32_t count = 0;

NDPixel pixels(8, 16);

void setup() {
  OCR0A = 0xAF; //Set Timer 0 Counter Register Value
  TIMSK0 |= _BV(OCIE0A);

  pixels.setBrightness(1);
  
  for (int pin : buttonRows) {
    pinMode(pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(pin), pressHandler, HIGH);
  }
  for (int pin : buttonCols) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
  showLedArray();
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
          color[i] = {red[i]};
        }
      //Player 2 King
      }else if(ledColor[showRow][showCol] == 4){
        for(int i = 0; i < 3; i++){
          color[i] = {red[i]};
        }
      }
      pixels.setPixel(showRow*4 + testCol, color[0], color[1], color[2]);
      /*
      Serial.println("Position");
      Serial.println(showRow);
      Serial.println(showCol);
      Serial.println("Color");
      Serial.println(color[0]);
      Serial.println(color[1]);
      Serial.println(color[2]);
      Serial.println("  ");*/
    }
  }
}

void keystroke(int row, int col) {
  //Serial.print("Pressed r:");
  //Serial.print(row);
  //Serial.print(" c:");
  //Serial.println(col);
  for(int i = 0; i < 16; i++) {
    pixels.setPixel(i, 0,0,0);
  }

  
  //Checkerboard code
  int firstPressedx = 0;
  int firstPressedy = 0;

  
  int showRow = row;
  int showCol;
  if(row%2) {
        showCol = 7 - col;
  } else {
        showCol = col;
  }
  if(ledColor[showRow][showCol] == 1){
    ledColor[showRow][showCol] = 0;
  }else if(ledColor[showRow][showCol] == 0){
    ledColor[showRow][showCol] = 1;
  }
  
  showLedArray();
  
  /*if(row%2) {
    col = 3 - col;
  }
  
  pixels.setPixel(row*4 + col, 255, 255, 255);*/
  
}


void pressHandler() {
  if(count > threshold) {
    count = 0;
    //Serial.println("Press detected!");
  
    int row = 0;
    for(int pin : buttonRows) {
      if(digitalRead(pin) == HIGH) {
        
        int col = 0;
        for(int pin2 : buttonCols) {
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
