#include "MovingObject.h"

void MovingObject::Update(float time)
{
  GameObject::Update(time);

  bool flyingFromOutside =
    _position.X() < 0.f ||
    _position.X() > 1.f ||
    _position.Y() < 0.f ||
    _position.Y() > 1.f;

  _position += _speed        * time;
  _speed    += _acceleration * time;

  if(!flyingFromOutside)
  {
    if     (_position.X() > 1.f)
      _position.X() -= 1.f;

    else if(_position.X() < 0.f)
      _position.X() += 1.f;

    if     (_position.Y() > 1.f)
      _position.Y() -= 1.f;

    else if(_position.Y() < 0.f)
      _position.Y() += 1.f;
  }

  if(_destroyed)
    _destroyedSAgo += time;
}

MovingObject::MovingObject(IDType id, std::string texture)
: GameObject(id, texture)
{
}
