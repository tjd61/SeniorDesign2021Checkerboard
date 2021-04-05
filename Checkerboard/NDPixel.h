#ifndef NDPixel_h
#define NDPixel_h

class NDPixel {
  int _NUM_LEDS = 1;
  int _PIN = B0;

  int _NUM_BYTES;
  const int _NUM_BITS = 8;
  
  byte* _COLOR_ARRAY = NULL;
  byte _BRIGHTNESS = 10;

  unsigned long _latchingPeriod = 10L; // 50 us (microseconds) reccomended
  unsigned long _lastUpdate;

public:
  NDPixel(int pin, int numLEDs);

  void setPixel(int pos, int R, int G, int B);
  void setColor(int pos, int R, int G, int B);
  void setAll(int R, int G, int B);
  void setBrightness(int newBrightness);
  void updateLEDs(void);
  void demo(void);
  void demoChase(int R, int G, int B, int len);
  
};
#endif
