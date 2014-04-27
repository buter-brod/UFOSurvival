#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "GameObject.h"

class Collider
{
public:

  static bool isBBoxOverlapping(GameObject &obj1, GameObject &obj2);

};

#endif