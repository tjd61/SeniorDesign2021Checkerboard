#define RXPIN 2
#define TXPIN 3

void bleInitialize(void);
void bleName(String newName);
void bleWrite(String charString);
String bleRead(void);
void bleDebug(void);
