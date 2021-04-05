#ifndef HM10_h
#define HM10_h
#define RXPIN 2
#define TXPIN 3

void bleInitialize(void);
void bleWrite(String charString);
String bleRead(void);
void bleDebug(void);

#endif
