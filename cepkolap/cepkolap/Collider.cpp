#include "Collider.h"

bool Collider::isBBoxOverlapping(GameObject &obj1, GameObject &obj2)
{
  //non-scaled bbox of any GameObject is assumed as -1..1

  Point obj1LB = obj1.GetPosition() - obj1.GetSize() / 2.f;
  Point obj1RT = obj1.GetPosition() + obj1.GetSize() / 2.f;

  Point obj2LB = obj2.GetPosition() - obj2.GetSize() / 2.f;
  Point obj2RT = obj2.GetPosition() + obj2.GetSize() / 2.f;

  return !( obj1RT.X() < obj2LB.X() || 
            obj1LB.X() > obj2RT.X() ||
            obj1RT.Y() < obj2LB.Y() ||
            obj1LB.Y() > obj2RT.Y()
          );
}
