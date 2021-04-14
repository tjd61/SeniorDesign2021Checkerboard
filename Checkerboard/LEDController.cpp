#include "Checkerboard.h"
#include "NDPixel.h"
#include "LEDController.h"

NDPixel pixels(Pin14, 64);

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
