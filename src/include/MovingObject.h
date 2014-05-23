#ifndef _MOVINGOBJECT_H_
#define _MOVINGOBJECT_H_

#include "GameObject.h"

class MovingObject :
  public GameObject
{
public:

  MovingObject(IDType id, std::string texture);
  
  virtual void Update(float dt);

  Point GetSpeed() const {return _speed;}

  void SetAcceleration(Point ac) {_acceleration =  ac;}
  void SetSpeed       (Point sp) {_speed        =  sp;}

protected:
  Point _speed;
  Point _acceleration;
};

DECLARE_PTR(MovingObject);

#endif