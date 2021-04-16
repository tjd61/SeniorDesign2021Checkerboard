#ifndef LEDController_h
#define LEDController_h

static int white[3]   = {255, 255, 255};
static int black[3]   = {0,   0,   0};
static int blue[3]    = {0,   0,   255};
static int red[3]     = {255, 0,   0};
static int cyan[3]    = {100, 255, 255};
static int green[3]   = {0,   255, 0};
static int yellow[3]  = {255, 255, 0};
static int lblue[3]   = {0,   0,   128};
static int lred[3]   = {128,   0,  0};

//1 for player 1 piece
//2 for player 2 piece
//3 for player 1 king
//4 for player 2 king

/*int ledColor[16] = {1, 2, 0, 0, 
                    0, 1, 2, 0, 
                    0, 0, 1, 2, 
                    0, 0, 0, 1,};*/

/*static int ledColor[8][8] = *//*{{1, 1, 1, 1, 2, 2, 2, 2,},
                      {3, 3, 3, 3, 4, 4, 4, 4,},
                      {1, 1, 1, 1, 2, 2, 2, 2,},
                      {3, 3, 3, 3, 4, 4, 4, 4,},
                      {1, 1, 1, 1, 2, 2, 2, 2,},
                      {3, 3, 3, 3, 4, 4, 4, 4,},
                      {1, 1, 1, 1, 2, 2, 2, 2,},
                      {3, 3, 3, 3, 4, 4, 4, 4,},};*/
                     /*{{1, 0, 1, 0, 1, 0, 1, 0,},
                      {0, 1, 0, 1, 0, 1, 0, 1,},
                      {1, 0, 1, 0, 1, 0, 1, 0,},
                      {0, 0, 0, 0, 0, 0, 0, 0,},
                      {0, 0, 0, 0, 0, 0, 0, 0,},
                      {0, 2, 0, 2, 0, 2, 0, 2,},
                      {2, 0, 2, 0, 2, 0, 2, 0,},
                      {0, 2, 0, 2, 0, 2, 0, 2,},};*/

//{{1, 0, 1, 0, 1, 0, 1, 0,},
// {0, 1, 0, 1, 0, 1, 0, 1,},
// {1, 0, 1, 0, 1, 0, 1, 0,},
// {0, 0, 0, 0, 0, 0, 0, 0,},
// {0, 0, 0, 0, 0, 0, 0, 0,},
// {0, 2, 0, 2, 0, 2, 0, 2,},
// {2, 0, 2, 0, 2, 0, 2, 0,},
// {0, 2, 0, 2, 0, 2, 0, 2,},};

void setupLeds(void);
void showLedArray(struct Game *);
void turnLEDOFF(int, int);
void updateLEDArray(void);

#endif
