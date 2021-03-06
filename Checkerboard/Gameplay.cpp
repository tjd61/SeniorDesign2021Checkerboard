#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Gameplay.h"
#include "Buttons.h"
#include "LEDController.h"
#include "HM10.h"
#include <Wire.h>

//open powershell to run
//then in Visual Studio Code click 'Terminal'->'Run Task' ... gcc.exe ... then in Powershell type  ./Gameplay while in the correct directory
//OR
//in powershell type "gcc Gameplay.c" -> "./a" because it default calls the executable 'a.exe'

//send Nicho list of valid spaces before each speech input is processed so he can use that info to help determine what the player said

byte moves[16][4] = {0}, n=0; //8 arrays of 4 numbers each -- in the form [x0,y0,x1,y1]
boolean speechEnabled = false;

void Play_Game(struct Game *p){
    while(1){
        while(1){

            if(p->must_jump != 1){   //if this is not a consecutive jump
                GetInput(p, 1);

                //this is for testing only
                if (SetTestBoard(p) == 1) break;

                if(CheckInputOne(p) == 1) break;
                else {
                    #ifdef TERMINAL
                        printf("\nYour piece is not on that space\n");
                    #else
                            invalidMoveMade(p);
                    #endif
                }
            }
            else { //this is a consecutive jump...so the space they are moving is the previous destination
                //---------------------------------make it so they can choose to jump again or not by selecting the same piece. if the other player selects a piece then let that player do that also-----------------------------------
                while(1){
                    #ifdef TERMINAL
                        printf("\nConsecutive jump available\n");
                        printf("\nInput starting coordinates (if consecutive jumping, must pick same piece)(either player can go): ");
                    
                    #endif

                    //scan in the coords
                    //check coords
                    //jumping player must choose same piece and jump available piece
                    //or other player can choose their peice and change to their turn, set must_jump=0
                    GetInput(p,1);
  
                    if(p->board[p->y0][p->x0] == 0 || ( (p->turn == p->board[p->y0][p->x0] || p->turn == p->board[p->y0][p->x0]-2 ) && (p->x0 != p->x1 && p->y0 != p->y1) ) ){
                        #ifdef TERMINAL
                            printf("\nInvalid coordinate.  Must choose same piece to jump or other player choose their piece\n");
                        #else
                            invalidMoveMade(p);
                        #endif
                    }
                    else {
                        break;
                    }
                }

                if(p->x0 == p->x1 && p->y0 == p->y1) { //player is jumping again
                    break;

                }
                else { //other players turn
                    SwitchTurn(p);
                    p->must_jump = 0;
                    break;
                }
            }
        }

        while(1){
            
            //for testing purposes
            if(p->x0 == 8){
                p->x1 = 0;
                p->y1 = 0;
                break;
            }
            if(!speechEnabled){
              GetInput(p,2);
              //process jumping a piece here before checking the second coords
              
            }

            CheckJump(p);

            //if 2nd coords are good continue, if not re enter coords
            if(CheckInputTwo(p)== 1) break;
            else if(CheckInputTwo(p) == 0) {
                #ifdef TERMINAL
                    printf("\nInvalid destination\n");
                #else
                    invalidMoveMade(p);
                #endif
            }
            else break;
        }

        if(CheckInputTwo(p) != 2) { //if coords2 != coords1 then continue
            break;
        }
    }

    //update and print board
    UpdateBoard(p);
    PrintBoard(p);
    //check is consecutive jumps is possible
    if(!speechEnabled){
      CheckMultipleJumps(p);
    }
    SwitchTurn(p);
}




void SetBoard(struct Game *p){
    
    for (int j=0; j<8; j=j+2){
        p->board[0][j] = 1;
        p->board[2][j] = 1;
        p->board[1][j+1] = 1;
    }

    for (int j=0; j<8; j=j+2){
        p->board[5][j+1] = 2;
        p->board[7][j+1] = 2;
        p->board[6][j] = 2;
    }

    p->turn = 1;

    #ifdef TERMINAL
        printf("\nTreat the bottom left corner as (0,0), top right corner as (7,7).\nWhen asked to input coordinates, enter with this format...\nx y \n(type x coord then hit space then type y coord then hit enter)");
        printf("\nTo set up jumping scenario...type 8 8 for the first player one coordinates");
    
    #endif
}


void SwitchTurn(struct Game *p){
     if (p->turn==1)
        p->turn=2;
    else
        p->turn=1;
}


int CheckInputOne(struct Game *p){
    if(p->board[p->y0][p->x0] != p->turn && ((p->turn == 1 && p->board[p->y0][p->x0] !=3) || (p->turn == 2 && p->board[p->y0][p->x0] != 4) ))
        return 0;
    else
        return 1;
}


int CheckInputTwo(struct Game *p){
    //if this is a consecutive jump, they MUST jump a piece
    if(p->must_jump == 1){
        if(p->jump != 1){
            #ifdef TERMINAL
                printf("\nMust jump available piece\n");
            
            #endif

            return 0;
        }
    }
    if(p->x0 == p->x1 && p->y0 == p->y1){ //same as first coords...choose different first coords
        PrintBoard(p);
        return 2;
    }else if(p->board[p->y1][p->x1] != 0 || abs(p->x0 - p->x1) != abs(p->y0 - p->y1) || (p->turn == 1 && p->board[p->y0][p->x0] != 3 && p->y1-p->y0 < 0 ) || (p->turn == 2 && p->board[p->y0][p->x0] != 4 && p->y1-p->y0 > 0) || ( (abs(p->x0-p->x1) != 1) && (abs(p->y0-p->y1) != 1) && p->jump == 0)) //rule checking should be good for normal pieces
        return 0;
    else
        return 1;
    
}

//check if jump is possible and valid
void CheckJump(struct Game *p){
    p->jump = 0;
    if(abs(p->x1 - p->x0) == 2 && abs(p->y1 - p->y0) == 2){ //player jumped a piece
        p->jump=1;
        //find where the jumped piece is
        int sign_x, sign_y;

        //set sign variables
        if(p->x1-p->x0 <0)
            sign_x = -1;
        else
            sign_x = 1;

        if(p->y1-p->y0 <0)
            sign_y = -1;
        else
            sign_y = 1;

        p->xj = sign_x + p->x0;
        p->yj = sign_y + p->y0;

        if(p->board[p->yj][p->xj] == 0)
            p->jump = 0;
    }

}


void CheckMultipleJumps(struct Game *p){
    //check if another jump is available...if one is, then let that player go again 
    //and jump a piece but they CANNOT NOT jump a piece and they must choose the same piece
    if(p->jump == 1){ //if the player just jumped a piece, they are elligible for a consecutive jump
        //cycle to the spaces around the piece and see if there is a jumpable opposite piece
        #ifdef TERMINAL
            printf("\nchecking multiple jumps\n");
        #endif
        if(p->turn == 1){
            printf("\nchecking jumps for p1...\n");

            //player 1 king and jumping backwards
            if(p->board[p->y1][p->x1] == 3){
                if(p->y1 > 1 && p->x1 > 1 && (p->board[p->y1-1][p->x1-1] == 2 || p->board[p->y1-1][p->x1-1] == 4)){
                    if(p->board[p->y1-2][p->x1-2] == 0){ //jump available over this piece
                        #ifdef TERMINAL
                            printf("\nAnother jump available\n");
                        #endif
                        SwitchTurn(p);
                        p->must_jump =1;
                        return;
                    }else{ //no consecutive jump over this piece
                        #ifdef TERMINAL
                            printf("\nNo more jumps available\n");
                        #endif
                        p->must_jump = 0;
                        p->jump = 0;
                    }
                
                } if(p->y1 > 1 && p->x1 < 6 && (p->board[p->y1-1][p->x1+1] == 2 || p->board[p->y1-1][p->x1+1] == 4)){
                    if(p->board[p->y1-2][p->x1+2] == 0){ //jump available over this piece
                        #ifdef TERMINAL
                            printf("\nAnother jump available\n");
                        #endif
                        SwitchTurn(p);
                        p->must_jump =1;
                        return;
                    }else{ //no consecutive jump over this piece
                        #ifdef TERMINAL
                            printf("\nNo more jumps available\n");
                        #endif
                        p->must_jump = 0;
                        p->jump = 0;
                    }
                    
                }  

            
                
            //player 1 jumping forward to the left
            }else if(p->y1 < 6 && p-> x1 > 1 && (p->board[p->y1+1][p->x1-1] == 2 || p->board[p->y1+1][p->x1-1] == 4)){
                if(p->board[p->y1+2][p->x1-2] == 0){ //possible jump available
                    #ifdef TERMINAL
                        printf("\nAnother jump available1\n");
                    #endif
                    SwitchTurn(p);
                    p->must_jump =1;
                    return;
                }else{ //no consecutive jump available
                    #ifdef TERMINAL
                        printf("\nNo more jumps available\n");
                    #endif
                    p->must_jump = 0;
                    p->jump = 0;
                }
            //player 1 jumping forward to the right   
            } if(p->y1 < 6 && p->x1 < 6 && (p->board[p->y1+1][p->x1+1] == 2 || p->board[p->y1+1][p->x1+1] == 4)){
                if(p->board[p->y1+2][p->x1+2] == 0){ //possible jump available
                    #ifdef TERMINAL
                        printf("\nAnother jump available2\n");
                    #endif
                    SwitchTurn(p);
                    p->must_jump =1;
                    return;
                }else{ //no consecutive jump available
                    #ifdef TERMINAL
                        printf("\nNo more jumps available\n");
                    #endif
                    p->must_jump = 0;
                    p->jump = 0;
                }
            } 

        //checking player 2 jumps
        }else if(p->turn == 2){
            #ifdef TERMINAL
                printf("\nchecking jumps for p2...\n");
            #endif

            //player 2 king jumping backwards
            if(p->board[p->y1][p->x1] == 4){
                if(p->y1 < 6 && p-> x1 > 1 && (p->board[p->y1+1][p->x1-1] == 1 || p->board[p->y1+1][p->x1-1] == 3)){
                    if(p->board[p->y1+2][p->x1-2] == 0){ //possible jump available
                        #ifdef TERMINAL
                            printf("\nAnother jump available1\n");
                        #endif
                        SwitchTurn(p);
                        p->must_jump =1;
                        return;
                    }else{ //no consecutive jump available+
                        #ifdef TERMINAL
                            printf("\nNo more jumps available\n");
                        #endif
                        p->must_jump = 0;
                        p->jump = 0;
                    }
                } else{
                    #ifdef TERMINAL
                        printf("\nNo more jumps available\n");
                    #endif
                    p->must_jump = 0;
                    p->jump = 0;
                }   
             if(p->y1 < 6 && p->x1 < 6 && (p->board[p->y1+1][p->x1+1] == 1 || p->board[p->y1+1][p->x1+1] == 3)){
                if(p->board[p->y1+2][p->x1+2] == 0){ //possible jump available
                    #ifdef TERMINAL
                        printf("\nAnother jump available2\n");
                    #endif
                    SwitchTurn(p);
                    p->must_jump =1;
                    return;
                }else{ //no consecutive jump available
                    #ifdef TERMINAL
                        printf("\nNo more jumps available\n");
                    #endif
                    p->must_jump = 0;
                    p->jump = 0;
                }            
             }
            }else if(p->y1 > 1 && p->x1 > 1 && (p->board[p->y1-1][p->x1-1] == 1 || p->board[p->y1-1][p->x1-1] == 3)){
                if(p->board[p->y1-2][p->x1-2] == 0){ //jump available over this piece
                    #ifdef TERMINAL
                        printf("\nAnother jump available\n");
                    #endif
                    SwitchTurn(p);
                    p->must_jump =1;
                    return;
                }else{ //no consecutive jump over this piece
                    #ifdef TERMINAL
                        printf("\nNo more jumps available\n");
                    #endif
                    p->must_jump = 0;
                    p->jump = 0;
                }
                
            } if(p->y1 > 1 && p->x1 < 6 && (p->board[p->y1-1][p->x1+1] == 1 || p->board[p->y1-1][p->x1+1] == 3)){
                if(p->board[p->y1-2][p->x1+2] == 0){ //jump available over this piece
                    #ifdef TERMINAL
                        printf("\nAnother jump available\n");
                    #endif
                    SwitchTurn(p);
                    p->must_jump =1;
                    return;
                }else{ //no consecutive jump over this piece
                    #ifdef TERMINAL
                        printf("\nNo more jumps available\n");
                    #endif
                    p->must_jump = 0;
                    p->jump = 0;
                }
                
            } 
        }
    }
    p->must_jump = 0;

}


void UpdateBoard(struct Game *p){
    
    if(p->x0 == 8)  //for testing
        return;

    p->board[p->y1][p->x1] = p->board[p->y0][p->x0];
    p->board[p->y0][p->x0] = 0;
    #ifdef TERMINAL
        printf("\nstart: (%d , %d)\t end: (%d , %d)",p->x0,p->y0,p->x1,p->y1);
    #endif
    if(p->jump == 1){
        p->board[p->yj][p->xj] = 0;
        #ifdef TERMINAL
            printf("\nJumped piece at %d %d",p->xj,p->yj);
        #endif
    }
    //check for kinging
    for( int i=0; i<8; i++){
        if(p->board[7][i] == 1){
            p->board[7][i] = 3;
            #ifdef TERMINAL
                printf("\nPlayer 1 has been kinged\n");
            #endif
        }
            
        if(p->board[0][i] == 2){
            p->board[0][i] = 4;
            #ifdef TERMINAL
                printf("\nPlayer 2 has been kinged\n");
            #endif
        }
    

    }
}


void PrintBoard(struct Game *p){
    #ifdef TERMINALt
        Serial.print("\n");
        for (int i=7; i>-1; i--){
            for(int j=0; j<8; j++){
                Serial.print(p->board[i][j]);
                Serial.print("  ");
            }
            Serial.println();
        }
        
        //Serial.print("\n----------------------\n\n");
    
    #else
       showLedArray(p);
    #endif
}


void GetInput(struct Game *p, int coords){
    
    #ifdef TERMINAL
         if(coords == 1){
            if(p->must_jump != 1)
                printf("\nPlayer %d turn\nInput Starting coords: ",p->turn);
            scanf("%d %d", &(p->x0), &(p->y0));
         }else if(coords == 2){
            printf("\nInput ending coords (type starting coords to go back and pick a new piece): ");
            scanf("%d %d", &(p->x1), &(p->y1));
         }
    
    #else
        //wait for interrupt from buttons or microphone button or bluetooth
        //Dalton - read button input here
        //if coords == 1 then load input to x0 and y0 like above
        //if coords == 2 then load input to x1 and y1 like above
        Serial.println("\n Sending Possible Moves");
        PossibleMoves(p); //gets possible moves and sends to slave
        //Three input status values
        int buttonStatus = pressHandler(coords, p);
        //Each status needs updated
        //int bluetoothStatus = bluetoothCommandParser(coords, p);
        int bluetoothStatus = 0;
        //int bluetoothStatus = bluetoothCommandParser(coords, p);
        int speechStatus = 0;
        /*if(coords == 1){
          speechStatus = SpeechHandler(p);
          speechEnabled = true;
        }else{
          speechStatus = 0;
        }*/
        
        int toggleBit = 0;
        while(buttonStatus == 0 && bluetoothStatus == 0 && speechStatus == 0){
          if(coords == 1){
            buttonStatus = pressHandler(coords, p);
            //Each status needs updated inside the loop
            
            //bluetoothStatus = bluetoothCommandParser(coords, p);
            
            speechStatus = SpeechHandler(p);
            if(speechStatus == 1){
              speechEnabled = true;
            }else{
              speechEnabled = false;
            }
          }else if(coords == 2 && !speechEnabled){
            int startTime = millis();
            int stopTime = millis();
            
            //Turn LED off
            if(toggleBit == 0){
              int col = p->x0;
              int row = p->y0;
              turnLEDOFF(row, col);
              toggleBit = 1;
            //Turn LED on
            }else{
              PrintBoard(p);
              toggleBit = 0;
            }

            //Change 500 to the time in milliseconds for the delay between sequential button presses
            while((stopTime - startTime) < 750){
              stopTime = millis();
              buttonStatus = pressHandler(coords, p);
              
              //bluetoothStatus = bluetoothCommandParser(coords, p);
              
              if(buttonStatus == 1 || bluetoothStatus == 1 || speechStatus == 1){
                break;
              }
            }
          }
        }

        
    #endif


}

int SetTestBoard(struct Game *p){
        if(p->x0 == 8){
            int c[8][8] = {{0,0,1,0,1,0,0,0},{0,1,0,0,0,2,0,0},{0,0,0,0,0,0,0,0},{0,2,0,1,0,2,0,1},{0,0,0,0,0,0,2,0},{0,0,0,0,0,0,0,0},{0,0,1,0,0,0,2,0},{0,2,0,0,0,0,0,0}};
            #ifdef TERMINAL
                printf("\nsetting up jumping scenario...\n");
            #endif
            for( int i=0; i<8; i++){
                for (int j=0; j<8; j++){
                    p->board[i][j] = c[i][j];
                }
            }
            return 1;
        }
        else
            return 0;
}

void invalidSound(){
  /*delay(500);
  tone(A0, 500, 200);
  delay(200);
  tone(A0, 1200, 200);*/
  delay(200);
  tone(A0, 300, 200);
  /*delay(200);
  tone(A0, 1000, 200);
  delay(200);
  tone(A0, 400, 200);
  delay(200);
  tone(A0, 1100, 200);*/
}

void invalidMoveMade(struct Game *p){

  int player = p->turn;
  
  //Flash LEDs color of player
  setAllLeds(player);

  //Made invalid noise
  invalidSound();
  
  //Update board to memory array
  PrintBoard(p);
}

void PossibleMoves(struct Game *p){
    //for players turn, scan through board and determine all possible moves (both sets of coords)
    
    int n=0;
    moves[16][4] = {0};
    for( int i=0; i<8; i++){
        for (int j=0; j<8; j++){

                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //change y1 x1 to i j DONE
                //remove writing variables DONE
                //remove switch turn functions DONE
                //can delete else clauses where no more jumps available DONE
                //add possible moves to array as (j,i) DONE
                //increment index n DONE

            //check possible jumps 
            if(p->turn == 1){
            

                //player 1 king and jumping backwards
                if(p->board[i][j] == 3){
                    if(i > 1 && j > 1 && (p->board[i-1][j-1] == 2 || p->board[i-1][j-1] == 4)){
                        if(p->board[i-2][j-2] == 0){ //jump available over this piece
                            moves[n][0] = j;
                            moves[n][1] = i;
                            moves[n][2] = j-2;
                            moves[n][3] = i-2;
                            n++;                               
                        }
                    
                    } if(i > 1 && j < 6 && (p->board[i-1][j+1] == 2 || p->board[i-1][j+1] == 4)){
                        if(p->board[i-2][j+2] == 0){ //jump available over this piece
                            moves[n][0] = j;
                            moves[n][1] = i;
                            moves[n][2] = j+2;
                            moves[n][3] = i-2;
                            n++;    
                        }
                        
                    }  

                
                    
                    //player 1 jumping forward to the left
                    }else if(i < 6 && p-> x1 > 1 && (p->board[i][j] == 1 || p->board[i][j] == 3) && (p->board[i+1][j-1] == 2 || p->board[i+1][j-1] == 4)){
                        if(p->board[i+2][j-2] == 0){ //possible jump available
                            moves[n][0] = j;
                            moves[n][1] = i;
                            moves[n][2] = j-2;
                            moves[n][3] = i+2;
                            n++;   
                        }
                    //player 1 jumping forward to the right   
                    } if(i < 6 && j < 6 && (p->board[i][j] == 1 || p->board[i][j] == 3) && (p->board[i+1][j+1] == 2 || p->board[i+1][j+1] == 4)){
                        if(p->board[i+2][j+2] == 0){ //possible jump available
                            moves[n][0] = j;
                            moves[n][1] = i;
                            moves[n][2] = j+2;
                            moves[n][3] = i+2;
                            n++;   
                        }
                    } 

            //checking player 2 jumps
            }else if(p->turn == 2){

                //player 2 king jumping backwards
                if(p->board[i][j] == 4){
                    if(i < 6 && p-> x1 > 1 && (p->board[i+1][j-1] == 1 || p->board[i+1][j-1] == 3)){
                        if(p->board[i+2][j-2] == 0){ //possible jump available
                            moves[n][0] = j;
                            moves[n][1] = i;
                            moves[n][2] = j-2;
                            moves[n][3] = i+2;
                            n++;   
                        }
                    }
                if(i < 6 && j < 6 && (p->board[i+1][j+1] == 1 || p->board[i+1][j+1] == 3)){
                    if(p->board[i+2][j+2] == 0){ //possible jump available
                        moves[n][0] = j;
                        moves[n][1] = i;
                        moves[n][2] = j+2;
                        moves[n][3] = i+2;
                        n++;   
                    }      
                }
                }else if(i > 1 && j > 1 && (p->board[i][j] == 2 || p->board[i][j] == 4) && (p->board[i-1][j-1] == 1 || p->board[i-1][j-1] == 3)){
                    if(p->board[i-2][j-2] == 0){ //jump available over this piece
                        moves[n][0] = j;
                        moves[n][1] = i;
                        moves[n][2] = j-2;
                        moves[n][3] = i-2;
                        n++;   
                    }
                    
                } if(i > 1 && j < 6 && (p->board[i][j] == 2 || p->board[i][j] == 4) && (p->board[i-1][j+1] == 1 || p->board[i-1][j+1] == 3)){
                    if(p->board[i-2][j+2] == 0){ //jump available over this piece
                        moves[n][0] = j;
                        moves[n][1] = i;
                        moves[n][2] = j+2;
                        moves[n][3] = i-2;
                        n++;   
                    }
                    
                } 
            }

            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //check possible 1 space moves

            //cycle through all spaces
            //if peice is a king, check if board[i+-1][j+-1] == 0
            //if peice is a normal piece, check if 2 forward diagnol spaces are available
            if(p->turn == 1){ //player 1
                if(p->board[i][j] == 3){ //king
                //check backward diagnols
                
                    if(i > 0 && j > 0 && (p->board[i-1][j-1] == 0)){ //space is available
                            moves[n][0] = j;
                            moves[n][1] = i;
                            moves[n][2] = j-1;
                            moves[n][3] = i-1;
                            n++;                          
                    
                    } if(i > 0 && j < 7 && (p->board[i-1][j+1] == 0)){ //space is available
                            moves[n][0] = j;
                            moves[n][1] = i;
                            moves[n][2] = j+1;
                            moves[n][3] = i-1;
                            n++;    
                    }    
                    
                } if(p->board[i][j] == 1 || p->board[i][j] == 3){ //normal piece
                //check forward diagnols
                    if(i < 7 && j < 7 && (p->board[i+1][j+1] == 0)){ //space is available
                            moves[n][0] = j;
                            moves[n][1] = i;
                            moves[n][2] = j+1;
                            moves[n][3] = i+1;
                            n++;                          
                    
                    } if(i < 7 && j > 0 && (p->board[i+1][j-1] == 0)){ //space is available
                            moves[n][0] = j;
                            moves[n][1] = i;
                            moves[n][2] = j-1;
                            moves[n][3] = i+1;
                            n++;    
                        }    
                    }
                }
             else{ //player 2
                if(p->board[i][j] == 4){ //king
                //check backward diagnols
                    if(i < 7 && j < 7 && (p->board[i+1][j+1] == 0)){ //space is available
                                moves[n][0] = j;
                                moves[n][1] = i;
                                moves[n][2] = j+1;
                                moves[n][3] = i+1;
                                n++;                          
                        
                        } if(i < 7 && j > 0 && (p->board[i+1][j-1] == 0)){ //space is available
                                moves[n][0] = j;
                                moves[n][1] = i;
                                moves[n][2] = j-1;
                                moves[n][3] = i+1;
                                n++;    
                        }    
                    
                } if(p->board[i][j] == 2 || p->board[i][j] == 4){ //normal piece
                //check forward diagnols
                    if(i > 0 && j > 0 && (p->board[i-1][j-1] == 0)){ //space is available
                            moves[n][0] = j;
                            moves[n][1] = i;
                            moves[n][2] = j-1;
                            moves[n][3] = i-1;
                            n++;                          
                    
                    } if(i > 0 && j < 7 && (p->board[i-1][j+1] == 0)){ //space is available
                            moves[n][0] = j;
                            moves[n][1] = i;
                            moves[n][2] = j+1;
                            moves[n][3] = i-1;
                            n++;    
                    }    

                }
            }
        
        }
    }
    
    //print possible moves
    #ifdef TERMINAL
        for (int k=0; k<16 ; k++){
            printf("\n [ (%d , %d) (%d , %d)", moves[k][0],moves[k][1],moves[k][2],moves[k][3]);
        }
    #endif
  
  //send to slave
  Wire.begin();        //join i2c bus
  Serial.begin(9600);
  //Wire.beginTransmission(8);  //transmit to device #4
  Serial.println("\nMaster transmits moves to slave");
  
  for(int i = 0; i<16; i++){
      Wire.beginTransmission(8);  //transmit to device #4
      Wire.write(moves[i][0]);
      Wire.write(moves[i][1]);
      Wire.write(moves[i][2]);
      Wire.write(moves[i][3]);
      Wire.endTransmission();
      Serial.print(moves[i][0]);
      Serial.print(moves[i][1]);
      Serial.print(moves[i][2]);
      Serial.print(moves[i][3]);
      Serial.println();
  }
  Serial.println("\nTransmission ended");
}

int SpeechHandler(struct Game *p){
  int index;
   Wire.requestFrom(8,1);    //request 1 bytes from slave device #8
  if(Wire.available()){
    index = Wire.read();
    if(index == 0){
      return 0;
    }
    index--;
    Serial.println("\nIndex received = ");
    Serial.println(index); 
    p->x0 = moves[index][0];
    p->y0 = moves[index][1];
    p->x1 = moves[index][2];
    p->y1 = moves[index][3];
    return 1;
  }else{
    return 0;
  }
  
}
