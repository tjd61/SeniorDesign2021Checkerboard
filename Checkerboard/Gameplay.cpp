#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Gameplay.h"

//open powershell to run
//then in Visual Studio Code click 'Terminal'->'Run Task' ... gcc.exe ... then in Powershell type  ./Gameplay while in the correct directory
//OR
//in powershell type "gcc Gameplay.c" -> "./a" because it default calls the executable 'a.exe'

//send Nicho list of valid spaces before each speech input is processed so he can use that info to help determine what the player said



void Play_Game(void){
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
            GetInput(p,2);

            //process jumping a piece here before checking the second coords
            CheckJump(p);

            //if 2nd coords are good continue, if not re enter coords
            if(CheckInputTwo(p)== 1) break;
            else if(CheckInputTwo(p) == 0) {
                #ifdef TERMINAL
                    printf("\nInvalid destination\n");
                
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
    CheckMultipleJumps(p);
    SwitchTurn(p);
}




void SetBoard(void){
    struct Game game = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0,0,0,0,0,0};
    struct Game *pt = &game;
    
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
    if(p->x0 == p->x1 && p->y0 == p->y1) //same as first coords...choose different first coords
        return 2;
    else if(p->board[p->y1][p->x1] != 0 || abs(p->x0 - p->x1) != abs(p->y0 - p->y1) || (p->turn == 1 && p->board[p->y0][p->x0] != 3 && p->y1-p->y0 < 0 ) || (p->turn == 2 && p->board[p->y0][p->x0] != 4 && p->y1-p->y0 > 0) || ( (abs(p->x0-p->x1) != 1) && (abs(p->y0-p->y1) != 1) && p->jump == 0)) //rule checking should be good for normal pieces
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
    #ifdef TERMINAL
        printf("\n");
        for (int i=7; i>-1; i--){
            for(int j=0; j<8; j++){
                printf("%d", p->board[i][j]);
                printf("  ");
            }
            printf("\n");
        }
        printf("\n----------------------\n\n");
    

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
        //wait for interrupt from buttons or microphone button or bluetooth
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
