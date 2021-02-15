#include <stdio.h>

//open powershell to run
//then in Visual Studio Code click 'Terminal'->'Run Task' ... gcc.exe ... then in Powershell type  ./Gameplay while in the correct directory

//change checkmove() or updateboard() to processmove() to do jumping pieces

struct Game{
    int board[8][8];
    int x0, y0, x1, y1;
    int turn;
};

//functions
void SetBoard(struct Game *);
int CheckInputOne(struct Game *);
int CheckInputTwo(struct Game *);
int CheckMove(int x_0, int y_0, int x_1, int y_1, int t);
void UpdateBoard(struct Game *);
void PrintBoard(struct Game *);



void main(void){
    struct Game game = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0,0};
    struct Game *pt = &game;

    SetBoard(pt);
    PrintBoard(&game);

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

        UpdateBoard(pt);
        PrintBoard(pt);
        if (pt->turn==1)
            pt->turn=2;
        else
            pt->turn=1;
        /*
        if(CheckMove(x0, y0, x1, y1, turn) == 1){
            UpdateBoard(x0, y0, x1, y1, turn);
            PrintBoard();
            if (turn==1)
                turn=2;
            else
                turn=1;
        } else{
            printf("\nThat move is not valid\n");
        } */
        
    }
}


void SetBoard(struct Game *p){

    for (int j=1; j<8; j=j+2){
        p->board[0][j] = 1;
        p->board[2][j] = 1;
        p->board[1][j-1] = 1;
    }

    for (int j=0; j<8; j=j+2){
        p->board[5][j] = 2;
        p->board[7][j] = 2;
        p->board[6][j+1] = 2;
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
    else if(p->board[p->y1][p->x1] != 0 || p->x0 == p->x1 || p->y0 == p->y1) //add other rules...only 1 space diagnol moves allowed unless jumping a piece
        return 0;
    else
        return 1;
    
}

/*int CheckMove(int x_0, int y_0, int x_1, int y_1, int t){
    if(board[y_0][x_0] != t || board[y_1][x_1] != 0 || ) //add rest of rules to check
        return 0;
    else
        return 1;
}*/

void UpdateBoard(struct Game *p){
    p->board[p->y0][p->x0] = 0;
    p->board[p->y1][p->x1] = p->turn;
    printf("start: (%d , %d)\t end: (%d , %d)",p->x0,p->y0,p->x1,p->y1);
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