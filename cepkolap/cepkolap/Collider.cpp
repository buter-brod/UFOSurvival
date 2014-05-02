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

bool Collider::isWithinCircle(Point& p, GameObject &polygon)
{
  return (pow(p.X() - polygon.GetPosition().X(), 2) / pow(polygon.GetSize().X(), 2)) +
         (pow(p.Y() - polygon.GetPosition().Y(), 2) / pow(polygon.GetSize().Y(), 2))

         <= 1.f;
}

bool PointInPolygon(Point& point, VArr& polygon, Point& pos, Point& size)
{
  bool c = false;
  unsigned int i, j;

  VArr szArr;
  for (Point pt : polygon)
    szArr.push_back(pos + pt * size);

  for(i = 0, j = szArr.size() - 1; i < szArr.size(); j = i++) 
  {
    if(((szArr[i].Y() >= point.Y()) != (szArr[j].Y() >= point.Y())) &&
      (point.X() <= (szArr[j].X() - szArr[i].X()) * (point.Y() - szArr[i].Y()) / (szArr[j].Y() - szArr[i].Y()) + szArr[i].X())
      )
      c = !c;
  }
  return c;
}

bool Collider::Collides(GameObject &obj1, GameObject &obj2, Point& p1, Point& p2)
{
  bool bboxOverlap = isBBoxOverlapping(obj1, obj2);

  if(!bboxOverlap)
    return false;

  if (obj2.GetVArray().empty())
    return true;

  float posX = obj1.GetPosition().X();
  float posY = obj1.GetPosition().Y();
  float hszX = obj1.GetSize().X() / 2.f;
  float hszY = obj1.GetSize().Y() / 2.f;

  VArr points = {Point(posX - hszX, posY - hszY),
                 Point(posX - hszX, posY + hszY),
                 Point(posX + hszX, posY + hszY),
                 Point(posX + hszX, posY - hszY)};

  for(Point p : points)
  {
    if(isWithinCircle(p, obj2))
    {
      if (!PointInPolygon(p, obj2.GetVArray(), obj2.GetPosition(), obj2.GetSize() / 2.f))
        continue;
      {        
        //...

        return true;
      }
        
    }
    else
      continue;
  }
  return false;
}