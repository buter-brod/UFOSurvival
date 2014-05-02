#include "Utils.h"
#include <sstream>
#include <random>

static const float RANDOM_STRENGTH = 5000.f;

unsigned int rnd()
{
  static std::default_random_engine rng(std::random_device{}());
  static std::uniform_real_distribution<float> dist(0, RANDOM_STRENGTH);
  return unsigned int(dist(rng));
}

float rnd01 ()        { return float(rnd()) / RANDOM_STRENGTH; }
float rnd0xf(float x) { return rnd01() * x; }
int   rnd0xi(int   x) { return rnd()   % x; }

float rndfMinMax(float min, float max) { return min + rnd01() * (max - min); }

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

Point Point::operator*(Point p)
{
  return Point(_x * p.X(), _y * p.Y());
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

void Point::operator-=(Point p)
{
  _x -= p.X();
  _y -= p.Y();
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