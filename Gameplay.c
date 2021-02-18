#include <stdio.h>
#include <stdlib.h>

//open powershell to run
//then in Visual Studio Code click 'Terminal'->'Run Task' ... gcc.exe ... then in Powershell type  ./Gameplay while in the correct directory

//multiple jumps should be done now
//need to add king functionality and king rule checking

struct Game{
    int board[8][8];
    int x0, y0, x1, y1, xj, yj;
    int turn;
    int jump, must_jump;
};

//functions
void SetBoard(struct Game *);
int CheckInputOne(struct Game *);
int CheckInputTwo(struct Game *);
void CheckJump(struct Game *);
void CheckMultipleJumps(struct Game *);
void SwitchTurn(struct Game *);
void UpdateBoard(struct Game *);
void PrintBoard(struct Game *);



void main(void){
    struct Game game = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0,0,0,0,0,0};
    struct Game *pt = &game;

    SetBoard(pt);
    PrintBoard(pt);

    while(1){
        while(1){
            while(1){

                if(pt->must_jump != 1){   //if this is not a consecutive jump
                    //get input coordinates
                    printf("\nPlayer %d turn\nInput Starting coords: ",pt->turn);
                    scanf("%d %d", &(pt->x0), &(pt->y0));

                    //this is for testing only
                    if(pt->x0 == 8){
                        int c[8][8] = {{0,0,1,0,0,1,0,0},{0,0,0,0,0,0,2,0},{0,0,1,0,0,0,0,0},{0,2,0,0,0,0,2,0},{0,0,0,0,1,0,0,0},{0,0,0,0,0,2,0,0},{0,0,0,0,0,0,0,0},{2,0,0,0,0,0,0,0}};
                        printf("\nsetting up jumping scenario...\n");
                        for( int i=0; i<8; i++){
                            for (int j=0; j<8; j++){
                                pt->board[i][j] = c[i][j];
                            }
                        }
                        
                        break;
                    }
                

                    if(CheckInputOne(pt) == 1)
                        break;
                    else
                        printf("\nYour piece is not on that space\n");
                }
                else { //this is a consecutive jump...so the space they are moving is the previous destination
                    printf("\nConsecutive jump available\n");
                    pt->x0 = pt->x1;
                    pt->y0 = pt->y1;
                    break;
                }
            }

            while(1){
                //for testing purposes
                
                if(pt->x0 == 8){
                    break;
                }

                printf("\nInput ending coords (type starting coords to go back and pick a new piece): ");
                scanf("%d %d", &(pt->x1), &(pt->y1));

                //process jumping a piece here before checking the second coords
                CheckJump(pt);

                //if 2nd coords are good continue, if not re enter coords
                if(CheckInputTwo(pt)== 1)
                    break;
                else if(CheckInputTwo(pt) == 0)
                    printf("\nInvalid destination\n");
                else
                    break;
            }

            if(CheckInputTwo(pt) != 2) //if coords2 != coords1 then continue
                break;
        }

        //update and print board
        UpdateBoard(pt);
        PrintBoard(pt);

        //check is consecutive jumps is possible
        CheckMultipleJumps(pt);

        SwitchTurn(pt);     
    }
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

    printf("\nTreat the bottom left corner as (0,0), top right corner as (7,7).\nWhen asked to input coordinates, enter with this format...\nx y \n(type x coord then hit space then type y coord then hit enter)");
    printf("\nTo set up jumping scenario...type 8 8 for the first player one coordinates");
}

void SwitchTurn(struct Game *p){
     if (p->turn==1)
        p->turn=2;
    else
        p->turn=1;
}

int CheckInputOne(struct Game *p){
    if(p->board[p->y0][p->x0] != p->turn)
        return 0;
    else
        return 1;
}


int CheckInputTwo(struct Game *p){
    //if this is a consecutive jump, they MUST jump a piece
    if(p->must_jump == 1){
        if(p->jump != 1){
            printf("\nMust jump available piece\n");
            return 0;
        }
    }
    if(p->x0 == p->x1 && p->y0 == p->y1) //same as first coords...choose different first coords
        return 2;
    else if(p->board[p->y1][p->x1] != 0 || abs(p->x0 - p->x1) != abs(p->y0 - p->y1) || (p->turn == 1 && p->y1-p->y0 < 0 ) || (p->turn == 2 && p->y1-p->y0 > 0) || ( (abs(p->x0-p->x1) != 1) && (abs(p->y0-p->y1) != 1) && p->jump == 0)) //rule checking should be good for normal pieces
        return 0;
    else
        return 1;
    
}

//check if jump is possible and valid
void CheckJump(struct Game *p){
    p->jump = 0;
    if(abs(p->x1 - p->x0) == 2 && abs(p->y1 - p->y0) == 2){
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
        printf("\nchecking multiple jumps\n");
        if(p->turn == 1){
            printf("\nchecking jumps for p1...\n");
            if(p->y1 < 6 && p-> x1 > 1 && p->board[p->y1+1][p->x1-1] == 2){
                if(p->board[p->y1+2][p->x1-2] == 0){ //possible jump available
                    printf("\nAnother jump available1\n");
                    SwitchTurn(p);
                    p->must_jump =1;
                }else{ //no consecutive jump available+
                    printf("\nNo more jumps available\n");
                    p->must_jump = 0;
                    p->jump = 0;
                }
                
            }else if(p->y1 < 6 && p->x1 < 6 && p->board[p->y1+1][p->x1+1] == 2){
                if(p->board[p->y1+2][p->x1+2] == 0){ //possible jump available
                    printf("\nAnother jump available2\n");
                    SwitchTurn(p);
                    p->must_jump =1;
                }else{ //no consecutive jump available
                    printf("\nNo more jumps available\n");
                    p->must_jump = 0;
                    p->jump = 0;
                }
            }else{ //no nearby opposite peices
                printf("\nNo more jumps available\n");
                p->must_jump = 0;
                p->jump = 0;
            }
        }else if(p->turn == 2){
            printf("\nchecking jumps for p2...\n");
            if(p->y1 > 1 && p->x1 > 1 && p->board[p->y1-1][p->x1-1] == 1){
                if(p->board[p->y1-2][p->x1-2] == 0){ //jump available over this piece
                    printf("\nAnother jump available\n");
                    SwitchTurn(p);
                    p->must_jump =1;
                }else{ //no consecutive jump over this piece
                    printf("\nNo more jumps available\n");
                    p->must_jump = 0;
                    p->jump = 0;
                }
                
            }else if(p->y1 > 1 && p->x1 < 6 && p->board[p->y1-1][p->x1+1] == 1){
                if(p->board[p->y1-2][p->x1+2] == 0){ //jump available over this piece
                    printf("\nAnother jump available\n");
                    SwitchTurn(p);
                    p->must_jump =1;
                }else{ //no consecutive jump over this piece
                    printf("\nNo more jumps available\n");
                    p->must_jump = 0;
                    p->jump = 0;
                }
                
            }else{ //no nearby opposite peices
                printf("\nNo more jumps available\n");
                p->must_jump = 0;
                p->jump = 0;
            }
        }
    }

}

void UpdateBoard(struct Game *p){
    
    if(p->x0 == 8)  //for testing
        return;

    p->board[p->y0][p->x0] = 0;
    p->board[p->y1][p->x1] = p->turn;
    printf("start: (%d , %d)\t end: (%d , %d)",p->x0,p->y0,p->x1,p->y1);
    if(p->jump == 1){
        p->board[p->yj][p->xj] = 0;
        printf("\nJumped piece at %d %d",p->xj,p->yj);
    }
}

void PrintBoard(struct Game *p){
    printf("\n");
    for (int i=7; i>-1; i--){
        for(int j=0; j<8; j++){
            printf("%d", p->board[i][j]);
            printf("  ");
        }
        printf("\n");
    }
    printf("\n----------------------\n\n");
}