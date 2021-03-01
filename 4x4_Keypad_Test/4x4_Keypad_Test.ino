#define R1 0
#define R2 1
#define R3 2
#define R4 3

#define C1 15
#define C2 14
#define C3 16
#define C4 10

int rows[4] = {R1, R2, R3, R4};
int cols[4] = {C1, C2, C3, C4};
volatile boolean pressed[4][4];

volatile static uint32_t threshold = 100000;
volatile static uint32_t count = 0;

void setup() {
  Serial.begin(9600);
  
  for (int pin : rows) {
    pinMode(pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(pin), pressHandler, HIGH);
  }
  for (int pin : cols) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
}


void loop() {
  noInterrupts();
  if(count < threshold) {
    count++;
    //Serial.println(count);
  }
  else if(count == threshold) {
    //Serial.println("Ready!");
    count++;
  }
  interrupts();
}


void keystroke(int row, int col) {
  Serial.print("Pressed r:");
  Serial.print(row);
  Serial.print(" c:");
  Serial.println(col);
}


void pressHandler() {
  noInterrupts();
  if(count == threshold + 1) {
    count = 0;
    Serial.println("Press detected!");
  
    int row = 0;
    for(int pin : rows) {
      if(digitalRead(pin) == HIGH) {
        
        int col = 0;
        for(int pin2 : cols) {
          digitalWrite(pin2, LOW);
          if (digitalRead(pin) == LOW) {
            keystroke(row, col);
          }
          digitalWrite(pin2, HIGH);
          col++;
        }
      }
      row++;
    }
  }
  interrupts();
}
