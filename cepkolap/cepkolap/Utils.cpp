#include "Utils.h"
#include <sstream>

std::string Utils::ToString(unsigned int i)
{
  std::stringstream ss;
  ss << i;
  return ss.str();
}

float dt(clock_t first, clock_t second)
{
  return (float(first) - float(second)) / CLOCKS_PER_SEC;
}

Point Point::operator/(float val)
{
  return Point(_x / val, _y / val);
}

Point Point::operator*(float val)
{
  return Point(_x * val, _y * val);
}

Point Point::operator+(Point p)
{
  return Point(_x + p.X(), _y + p.Y());
}

Point Point::operator-(Point p)
{
  return Point(_x - p.X(), _y - p.Y());
}

void Point::operator+=(Point p)
{
  _x += p.X();
  _y += p.Y();
}

void Point::operator*=(Point p)
{
  _x *= p.X();
  _y *= p.Y();
}

void Point::operator*=(float val)
{
  _x *= val;
  _y *= val;
}

Point Point::normalized()
{
  float inv_length = 1.0f / sqrt(_x *_x + _y *_y);
  return (*this * inv_length);
}