#include "Asteroid.h"
#include <math.h>

static const float PI   = 4.f * atan(1.f);
static const float PI_2 = PI / 2.f;

static const float ASTEROID_MIN_SPEED = 0.05f;
static const float ASTEROID_MAX_SPEED = 0.2f;
static const float ASTEROID_MAX_SIZE  = 0.2f;
static const float ASTEROID_MIN_SIZE  = 0.1f;

Asteroid::Asteroid(IDType id, std::string tex, VArr vertexArr, unsigned int lifes)
: GameObject(id, tex, lifes)
{
  if (vertexArr.empty())
    generatePoly();
  else
    _vertArray = vertexArr;

  float sizef = rndfMinMax(ASTEROID_MIN_SIZE, ASTEROID_MAX_SIZE);
  Point size(sizef, sizef);
  Point halfSize = size / 2.f;

  enum Side { LEFT, TOP, RIGHT, BOTTOM };
  Side side = Side(rnd0xi(4));
  Point pos;

  switch(side)
  {
  case LEFT:
    pos.X() = 0.f - halfSize.X();
    pos.Y() = rnd01();
    break;
  case TOP:
    pos.X() = rnd01();
    pos.Y() = 1.f + halfSize.Y();
    break;
  case RIGHT:
    pos.X() = 1.f + halfSize.X();
    pos.Y() = rnd01();
    break;
  case BOTTOM:
    pos.X() = rnd01();
    pos.Y() = 0.f - halfSize.Y();
    break;
  }

  Point direction(rnd01() * (1.f - 2.f * halfSize.X()) + halfSize.X(),
                  rnd01() * (1.f - 2.f * halfSize.Y()) + halfSize.Y());

  Point speed = (direction - pos).normalized() * rndfMinMax(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED);

  SetSize    (size);
  SetSpeed   (speed);
  SetPosition(pos);
}

void Asteroid::generatePoly()
{
  float curAngle = rnd01() * PI_2;

  while (curAngle < 2.f * PI)
  {
    float x = 1.f * cos(curAngle);
    float y = 1.f * sin(curAngle);

    Point p(x, y);
    _vertArray.push_back(p);

    curAngle += rnd01() * PI_2;
  }
}