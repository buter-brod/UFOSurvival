#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <time.h>

#define FPS 120.f

float dt(clock_t first, clock_t second);

class Point
{
public:
  Point()                : _x(0.f), _y(0.f) {}
  Point(float x, float y): _x(x),   _y(y)   {}

  Point operator* (float val);
  Point operator+ (Point p);
  Point operator- (Point p);
  void  operator+=(Point p);
  void  operator*=(Point p);
  void  operator*=(float val);
  Point normalized();

  float& X()    { return _x; }
  float& Y()    { return _y; }

  int I_X()  { return int(_x); }
  int I_Y()  { return int(_y); }
  
protected:
  float _x, _y;
};

class Utils
{
public:
  static std::string ToString(unsigned int i);

};

#endif