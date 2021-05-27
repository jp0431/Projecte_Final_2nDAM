#include "touchScreenPoint.h"

TouchScrPoint::TouchScrPoint(void) {
    x = y = 0; 
}

TouchScrPoint::TouchScrPoint(int16_t x0, int16_t y0) { 
    x = x0;  
    y = y0; 
}

bool TouchScrPoint::operator==(TouchScrPoint p1) {
  return ((p1.x == x) && (p1.y == y));
}

bool TouchScrPoint::operator!=(TouchScrPoint p1) {
  return ((p1.x != x) || (p1.y != y));
}