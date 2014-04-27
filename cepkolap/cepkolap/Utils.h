#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <time.h>
#include <cmath>

#define FPS 120.f

float rnd01 ();
float rnd0xf(float x);
int   rnd0xi(int   x);
float rndfMinMax(float min, float max);

/*
rnd01  is random value 0..1
rnd0xf  is random value 0..x
rnd0xi is random integer value 0..x
*/

float dt(clock_t first, clock_t second);

class Point
{
public:
  Point()                : _x(0.f), _y(0.f) {}
  Point(float x, float y): _x(x),   _y(y)   {}
  
  Point operator+ (Point p);
  Point operator- (Point p);
  void  operator+=(Point p);
  void  operator*=(Point p);
  
  void  operator*=(float val);
  Point operator* (float val);
  Point operator/ (float val);

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