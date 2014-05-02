#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "GameObject.h"

class Collider
{
public:
  
  static bool Collides(GameObject &obj1, GameObject &obj2, Point& p1, Point& p2);

protected:

  static bool isBBoxOverlapping(GameObject &obj1, GameObject &obj2);
  static bool isWithinCircle   (Point& p, GameObject &polygon);
};

#endif