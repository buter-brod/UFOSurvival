#include "Bullet.h"

void Bullet::Update(float time)
{
  MovingObject::Update(time);
  
  _lifeTime += time;

  if(_maxLifeTime > 0.f && _lifeTime > _maxLifeTime)
    Destroy();
}

Bullet::Bullet(IDType id, std::string texture)
: MovingObject(id, texture), _lifeTime(0.f)
{
}

