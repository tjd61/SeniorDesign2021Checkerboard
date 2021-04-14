#ifndef buttonController_h
#define buttonController_h

#include "Checkerboard.h"
#include "Gameplay.h"

//Most to least significant bit
//List of pins to be used for sweeping button rows
static int buttonTestRows[3] = {BR2, BR1, BR0};
//List of pins to be used for sweeping button columns
static int buttonTestCols[3] = {BC2, BC1, BC0};

void button_setup(void);
void keystroke(int, int, int);
void pressHandler(int);
#endif
