#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "GameObject.h"

class Collider
{
public:
  static bool CollidesPoly(GameObject &obj1, GameObject &poly, Collider::CrackSpots &crsp);
  
protected:
  
  static bool isWithinCircle   (Point& p, GameObject &polygon);
  static bool isBBoxOverlapping(GameObject &obj1, GameObject &obj2);
};

#endif