#ifndef _BULLET_H_
#define _BULLET_H_

#include "MovingObject.h"
#include <list>

class Bullet :
  public MovingObject
{
public:
  Bullet(IDType id, std::string texture);
  void Update(float time);
  void SetMaxLifeTime (float tm) {_maxLifeTime = tm;}

protected:

  float _lifeTime;
  float _maxLifeTime;
};

DECLARE_PTR(Bullet);
DECLARE_LST(Bullet);

#endif