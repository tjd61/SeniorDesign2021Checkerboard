#ifndef HM10_h
#define HM10_h

#define RXPIN 2
#define TXPIN 3

#include "Checkerboard.h"
#include "Gameplay.h"

void bleInitialize(void);
void bleWrite(String charString);
String bleRead(void);
void bleDebug(void);

int bluetoothCommandParser(int , struct Game*);
#endif
