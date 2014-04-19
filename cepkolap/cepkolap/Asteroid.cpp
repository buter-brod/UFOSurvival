#include "Asteroid.h"
#include <math.h>

static float PI = 4.f * atan(1.f);

#define PI_2 (PI / 2.f)
#define RND_01 ((float)rand() / (RAND_MAX))

Asteroid::Asteroid(std::string name)
: GameObject(std::string("Asteroid_") + name, "asteroid.png", 1)
{
  generatePoly();
}


Asteroid::~Asteroid()
{
}

void Asteroid::generatePoly()
{
  srand((unsigned int)time(0));
  float curAngle = RND_01 * PI_2;

  while (curAngle < 2.f * PI)
  {
    curAngle += RND_01 * PI_2;

    float x = 1.f * cos(curAngle);
    float y = 1.f * sin(curAngle);

    Point p(x, y);
    _vertArray.push_back(p);
  }
}