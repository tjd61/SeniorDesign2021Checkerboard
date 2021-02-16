#include <stdio.h>
#include <stdlib.h>

//open powershell to run
//then in Visual Studio Code click 'Terminal'->'Run Task' ... gcc.exe ... then in Powershell type  ./Gameplay while in the correct directory

//jumping works
//need to add king functionality

struct Game{
    int board[8][8];
    int x0, y0, x1, y1, xj, yj;
    int turn;
    int jump;
};

//functions
void SetBoard(struct Game *);
int CheckInputOne(struct Game *);
int CheckInputTwo(struct Game *);
void CheckJump(struct Game *);
void UpdateBoard(struct Game *);
void PrintBoard(struct Game *);



void main(void){
    struct Game game = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0,0,0,0,0};
    struct Game *pt = &game;

    SetBoard(pt);
    PrintBoard(pt);

    while(1){
        while(1){
            while(1){
                
            //get input coordinates
            printf("\nPlayer %d turn\nInput Starting coords: ",pt->turn);
            scanf("%d %d", &(pt->x0), &(pt->y0));
            if(CheckInputOne(pt) == 1)
                break;
            printf("\nYour piece is not on that space\n");
            }

            while(1){
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
        if (pt->turn==1)
            pt->turn=2;
        else
            pt->turn=1;
        
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

}

int CheckInputOne(struct Game *p){
    if(p->board[p->y0][p->x0] != p->turn)
        return 0;
    else
        return 1;
}


int CheckInputTwo(struct Game *p){
    if(p->x0 == p->x1 && p->y0 == p->y1) //same as first coords...choose different first coords
        return 2;
        //might be able to get rid of these "OR" statements and just have the abs ones
    else if(p->board[p->y1][p->x1] != 0 || abs(p->x0 - p->x1) != abs(p->y0 - p->y1) || ( (abs(p->x0-p->x1) != 1) && (abs(p->y0-p->y1) != 1) && p->jump == 0)) //rule checking should be good for normal pieces
        return 0;
    else
        return 1;
    
}

//check if jump is possible and valid
void CheckJump(struct Game *p){

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
    }

}

void UpdateBoard(struct Game *p){
    p->board[p->y0][p->x0] = 0;
    p->board[p->y1][p->x1] = p->turn;
    printf("start: (%d , %d)\t end: (%d , %d)",p->x0,p->y0,p->x1,p->y1);
    if(p->jump == 1){
        p->board[p->yj][p->xj] = 0;
        printf("\nJumped piece at %d %d",p->xj,p->yj);
        p->jump=0;
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