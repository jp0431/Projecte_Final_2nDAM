#ifndef TOUCH_SCREEN_POINT
#define TOUCH_SCREEN_POINT

typedef signed short int16_t;

class TouchScrPoint {
public:
  TouchScrPoint(void);
  TouchScrPoint(int16_t x, int16_t y);

  bool operator==(TouchScrPoint);
  bool operator!=(TouchScrPoint);

  int16_t x, y;  
};

#endif