#include "Checkerboard.h"
#include "NDPixel.h"
#include "LEDController.h"
#include "Gameplay.h"

NDPixel pixels(Pin14, 64);

void setupLeds(){
  pixels.setBrightness(1);
}

void showLedArray(struct Game *p) {
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
      if(p->board[showRow][showCol] == 0){
        for(int i = 0; i < 3; i++){
          color[i] = black[i];
        }
      //Player 1 Piece
      }else if(p->board[showRow][showCol] == 1){
        for(int i = 0; i < 3; i++){
          color[i] = blue[i];
        }
      //Player 2 Piece
      }else if(p->board[showRow][showCol] == 2){
        for(int i = 0; i < 3; i++){
          color[i] = red[i];
        }
      //Player 1 King
      }else if(p->board[showRow][showCol] == 3){
        for(int i = 0; i < 3; i++){
          color[i] = green[i];
        }
      //Player 2 King
      }else if(p->board[showRow][showCol] == 4){
        for(int i = 0; i < 3; i++){
          color[i] = yellow[i];
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
  pixels.updateLEDs();
}

void updateLEDArray(){
  pixels.updateLEDs();
}

void turnLEDOFF(int row, int col){
  int testOffCol = col;
  int showOffRow = row;
  int showOffCol;
  if(showOffRow%2 == 1) {
    showOffCol = 7 - testOffCol;
  } else {
    showOffCol = testOffCol;
  }
  pixels.setPixel(showOffRow*8 + showOffCol, black[0], black[1], black[2]);
  pixels.updateLEDs();
}

void setAllLeds(int player){
  if(player == 1){
    pixels.setAll(blue[0], blue[1], blue[2]);
  }else if(player == 2){
    pixels.setAll(red[0], red[1], red[2]);
  }
  pixels.updateLEDs();
}
