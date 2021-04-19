#ifndef gameplay_h
#define gameplay_h

//#define TERMINAL //execute this line if running in computer terminal...otherwise comment it out

struct Game{
    int board[8][8];
    int x0, y0, x1, y1, xj, yj;
    int turn, jump, must_jump;

};



static struct Game *p;

//functions
void SetBoard(struct Game *);
int CheckInputOne(struct Game *);
int CheckInputTwo(struct Game *);
void CheckJump(struct Game *);
void CheckMultipleJumps(struct Game *);
void SwitchTurn(struct Game *);
void UpdateBoard(struct Game *);
void PrintBoard(struct Game *);
void GetInput(struct Game *, int coords); //coords is either the 1st set of coordinates or the 2nd set of coordinates
int SetTestBoard(struct Game *);
void Play_Game(struct Game *);
void invalidSound(void);
void invalidMoveMade(struct Game *);
void PossibleMoves(struct Game *);
int SpeechHandler(struct Game *);

#endif
