#include "Arduino.h"
#include "NDPixel.h"

#define PORT     (PORTB)     // We're using pin 8, which on ATMega328 is in Port B 
//                Should be pin 14 of the ATmega328
#define PORT_PIN (PORTB0)    // Pin 8 is in Port B position 0 (9 is in position 1, etc)

NDPixel::NDPixel(int pin, int numLEDs) {
  _PIN = pin;
  _NUM_LEDS = numLEDs;
  _NUM_BYTES = _NUM_LEDS * 3;

  pinMode(_PIN, OUTPUT);
  digitalWrite(_PIN, LOW);

  if((_COLOR_ARRAY = (uint8_t *)malloc(_NUM_BYTES)))             
  {                 
    memset(_COLOR_ARRAY, 0, _NUM_BYTES);                         
  }
}

void NDPixel::setPixel(int pos, int R, int G, int B) {
  uint8_t *p = &_COLOR_ARRAY[pos*3]; 
  *p++ = (byte)map(G, 0, 255, 0, _BRIGHTNESS);
  *p++ = (byte)map(R, 0, 255, 0, _BRIGHTNESS);
  *p = (byte)map(B, 0, 255, 0, _BRIGHTNESS);
}

void NDPixel::setColor(int pos, int R, int G, int B) {
  if(R > 255) R = 255;
  if(G > 255) G = 255;
  if(B > 255) B = 255;
  if(R < 0) R = 0;
  if(G < 0) G = 0;
  if(B < 0) B = 0;

  setPixel(pos, R, G, B);
}

void NDPixel::setAll(int R, int G, int B) {
  for(int i = 0; i < _NUM_LEDS; i++) {
    setPixel(i, R, G, B);
  }
}

void NDPixel::setBrightness(int brightness) {
  if(brightness>255) brightness = 255;
  if(brightness<0) brightness = 0;
  _BRIGHTNESS = (byte)brightness;
}

void NDPixel::updateLEDs(void) {
  if(!_COLOR_ARRAY) return;
  while((micros() - _lastUpdate) < _latchingPeriod) {}
  
  cli(); // Disable interrupts so that timing is as precise as possible
  byte *p = _COLOR_ARRAY;   // Copy the start address of our data array
  byte val  = *p++;      // Get the current byte value & point to next byte
  byte high = PORT |  _BV(PORT_PIN); // Bitmask for sending HIGH to pin
  byte low  = PORT & ~_BV(PORT_PIN); // Bitmask for sending LOW to pin
  volatile byte tmp  = low;       // Swap variable to adjust duty cycle 
  volatile byte nbits= _NUM_BITS;  // Bit counter for inner loop
  byte nbytes = _NUM_BYTES; // Byte counter for outer loop
 
  asm volatile(
    "nextbit:\n\t"         // -    label                       (T =  0) 
    "sbi  %0, %1\n\t"     // 2    signal HIGH                 (T =  2) 
    "sbrc %4, 7\n\t"      // 1-2  if MSB set                  (T =  ?)          
     "mov  %6, %3\n\t"    // 0-1   tmp'll set signal high     (T =  4) 
    "dec  %5\n\t"         // 1    decrease bitcount           (T =  5) 
    "nop\n\t"             // 1    nop (idle 1 clock cycle)    (T =  6)
    "st   %a2, %6\n\t"    // 2    set PORT to tmp             (T =  8)
    "mov  %6, %7\n\t"     // 1    reset tmp to low (default)  (T =  9)
    "breq nextbyte\n\t"   // 1-2  if bitcount ==0 -> nextbyte (T =  ?)                
    "rol  %4\n\t"         // 1    shift MSB leftwards         (T = 11)
    "rjmp .+0\n\t"        // 2    nop nop                     (T = 13)
    "cbi   %0, %1\n\t"    // 2    signal LOW                  (T = 15)
    "rjmp .+0\n\t"        // 2    nop nop                     (T = 17)
    "nop\n\t"             // 1    nop                         (T = 18)
    "rjmp nextbit\n\t"    // 2    bitcount !=0 -> nextbit     (T = 20)
   "nextbyte:\n\t"        // -    label                       -
    "ldi  %5, 8\n\t"      // 1    reset bitcount              (T = 11)
    "ld   %4, %a8+\n\t"   // 2    val = *p++                  (T = 13)
    "cbi   %0, %1\n\t"    // 2    signal LOW                  (T = 15)
    "rjmp .+0\n\t"        // 2    nop nop                     (T = 17)
    "nop\n\t"             // 1    nop                         (T = 18)
    "dec %9\n\t"          // 1    decrease bytecount          (T = 19)
    "brne nextbit\n\t"    // 2    if bytecount !=0 -> nextbit (T = 20)
    ::
    // Input operands         Operand Id (w/ constraint)
    "I" (_SFR_IO_ADDR(PORT)), // %0
    "I" (PORT_PIN),           // %1
    "e" (&PORT),              // %a2
    "r" (high),               // %3
    "r" (val),                // %4
    "r" (nbits),              // %5
    "r" (tmp),                // %6
    "r" (low),                // %7
    "e" (p),                  // %a8
    "w" (nbytes)              // %9
  );
  
  sei(); //Re-enable interrupts
  _lastUpdate = micros();
}

void NDPixel::demo() { //Fade from OFF to Red to Green to Blue to OFF
  for(int i = 0; i < _NUM_LEDS; i++) {
    for (int j=0; j<255; j++){
      setColor(i, j, 0, 0);
      delay (1);
    }
    for (int j=0; j<255; j++){
      setColor(i, 255-j, j, 0);
      delay (1);
    }
    for (int j=0; j<255; j++){
      setColor(i, 0, 255-j, j);
      delay (1);
    }
    for (int j=255; j>0; j--){
      setColor(i, 0, 0, j);
      delay (1);
    }
  }
}

void NDPixel::demoChase(int R, int G, int B, int len) {
  for(int i = 0; i < _NUM_LEDS + len; i++) {
    if(i < _NUM_LEDS) setColor(i, R, G, B);
    if(i-len >= 0) setColor(i-len, 0, 0, 0);
    delay(50);
  }
}
