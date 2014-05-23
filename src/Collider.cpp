#include "Asteroid.h"

#include <algorithm>

bool isBBoxOverlapping(GameObjectPtr obj1, GameObjectPtr obj2)
{
  //non-scaled bbox of any GameObject is assumed as -1..1

  Point obj1LB = obj1->GetPosition() - obj1->GetSize() / 2.f;
  Point obj1RT = obj1->GetPosition() + obj1->GetSize() / 2.f;

  Point obj2LB = obj2->GetPosition() - obj2->GetSize() / 2.f;
  Point obj2RT = obj2->GetPosition() + obj2->GetSize() / 2.f;

  return !( obj1RT.X() < obj2LB.X() || 
            obj1LB.X() > obj2RT.X() ||
            obj1RT.Y() < obj2LB.Y() ||
            obj1LB.Y() > obj2RT.Y()
          );
}

bool isWithinCircle(Point& p, AsteroidPtr &polygon)
{
  return (pow(p.X() - polygon->GetPosition().X(), 2) / pow(polygon->GetSize().X(), 2)) +
         (pow(p.Y() - polygon->GetPosition().Y(), 2) / pow(polygon->GetSize().Y(), 2))

         <= 1.f;
}

VArr getRealPolygon(AsteroidPtr &polygon)
{
  VArr arr;
  for(Point pt : polygon->GetVArray())
    arr.push_back(polygon->GetPosition() + pt * polygon->GetSize() / 2.f);

  return arr;
}

bool pointInPolygon(Point& point, VArr& poly)
{
  bool c = false;
 
  for(size_t i = 0, j = poly.size() - 1; i < poly.size(); j = i++) 
  {
    if(
      ((poly[i].Y() >= point.Y()) != (poly[j].Y() >= point.Y())) &&
      (point.X() <= (poly[j].X() - poly[i].X()) * (point.Y() - poly[i].Y()) / (poly[j].Y() - poly[i].Y()) + poly[i].X())
      )
      c = !c;
  }
  return c;
}

struct Segment
{
  Segment(Point b, Point e): _begin(b), _end(e) {}
  Point _begin;
  Point _end;
};

bool crossLine(Segment seg1, Segment seg2, Point& cross)
{
  float dx1 = seg1._end.X() - seg1._begin.X();
  float dy1 = seg1._end.Y() - seg1._begin.Y();
  float dx2 = seg2._end.X() - seg2._begin.X();
  float dy2 = seg2._end.Y() - seg2._begin.Y();

  cross.X() = dy1 * dx2 - dy2 * dx1;
  if(cross.X() == 0.f || dx2 == 0.f)
    return false;

  cross.Y() =   seg2._begin.X() * seg2._end.Y() - seg2._begin.Y() * seg2._end.X();
  cross.X() = ((seg1._begin.X() * seg1._end.Y() - seg1._begin.Y() * seg1._end.X()) * dx2 - cross.Y() * dx1) / cross.X();
  cross.Y() = (dy2 * cross.X() - cross.Y()) / dx2;
  

  return
    ((seg1._begin.X() <= cross.X() && seg1._end  .X() >= cross.X()) || 
     (seg1._end  .X() <= cross.X() && seg1._begin.X() >= cross.X())) 
    && 
    ((seg2._begin.X() <= cross.X() && seg2._end  .X() >= cross.X()) || 
     (seg2._end  .X() <= cross.X() && seg2._begin.X() >= cross.X()));
}

size_t getCrosses(Point p, Point crossVec, AsteroidPtr& obj, VArr& realPolygon, CrackSpots& crsp)
{
  size_t crossCount = 0;

  for(size_t bInd = 0; bInd < realPolygon.size(); bInd++)
  {
    size_t eInd = (bInd + 1) % realPolygon.size();
    Point cross;

    if(crossLine(Segment(p + crossVec, p - crossVec), Segment(realPolygon[bInd], realPolygon[eInd]), cross))
    {
      cross -= obj->GetPosition();
      cross /= obj->GetSize() / 2.f; // back transformation to relative-to-polygon coordinates

      crsp[crossCount] = CrackSpot(cross, bInd);
      crossCount++;
    }
  }
  return crossCount;
}

bool CollidesPoly(MovingObjectPtr obj1, AsteroidPtr &poly, CrackSpots &crsp)
{
  /* obj1 is considered a 'bullet' here (rectangle), obj2 is an 'asteroid' (n-gon):
     first, check whether their bboxes overlap (isBBoxOverlapping), no need to continue, if they're not;
     then sequently take 4 vertices of rectangle (obj1) and check if any of them are within n-gon outcircle (outellipse actually, 'cause _size ratio may vary) 
     (isWithinCircle). If point lies inside, then we check its placement (pointInPolygon) with ''all edges on the same side of the point''-algorithm.
     After we've find out that point is inside the polygon, it's used as a center of a segment (obj1.Speed goes as direction for that) 
     that will split obj2-polygon into two parts (getCrosses) by iterating all polygon edges, crossing them with that speed-oriented segment. 
     Finally, only crosspoints (crsp) are returned, and no changes made to polygon here directly.

     Special case is when non of rectangle points lies in polygon, but polygon vertex crosses one of rectangle' sides. Works just the same.
  */

  if(!isBBoxOverlapping(obj1, poly))
    return false;

  VArr realPolygon = getRealPolygon(poly); // transform all poly vertices to world-coordinates

  float posX = obj1->GetPosition().X();
  float posY = obj1->GetPosition().Y();
  float hszX = obj1->GetSize().X() / 2.f;
  float hszY = obj1->GetSize().Y() / 2.f;

  enum {LB, LT, RT, RB};

  VArr points = {Point(posX - hszX, posY - hszY),  //LB
                 Point(posX - hszX, posY + hszY),  //LT
                 Point(posX + hszX, posY + hszY),  //RT
                 Point(posX + hszX, posY - hszY)}; //RB

  //using speed vector as direction of the future 'crack'. if out object is not moving, using inverted poly speed instead
  Point speedVec = obj1->GetSpeed().len() > 0.f ? 
       obj1->GetSpeed(): 
     - poly->GetSpeed();

  Point speedVecLong = speedVec.normalized() * std::max<float>(poly->GetSize().X(), 
                                                               poly->GetSize().Y()); // make our cross-segment long enough to cover whole polygon
  for(Point p : points)
  {
    if(!isWithinCircle(p, poly)) // check if point lies within the (-1..1) circle. This circle is the incircle for bbox and the outcircle for polygon.
      continue;

    if(!pointInPolygon(p, realPolygon))
      continue;

    getCrosses(p, speedVecLong, poly, realPolygon, crsp);
    return true;
  }

  // now handling a special case: polygon vertex is within 1st object bbox
  for (Point p : realPolygon)
  {
    if (p.X() > points[LT].X() && 
        p.X() < points[RT].X() &&
        p.Y() > points[LB].Y() &&
        p.Y() < points[LT].Y())

      for(size_t side = 0; side < points.size(); side++)
      {
        size_t nextV = (side + 1u) % points.size();
        Point cross;
        if(crossLine(Segment(p, poly->GetPosition()), Segment(points[side], points[nextV]), cross))
        {
          getCrosses(cross, speedVecLong, poly, realPolygon, crsp);
          return true;
        }
      }
  }
  return false;
}