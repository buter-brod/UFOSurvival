#include "GameObject.h"

GameObject::GameObject(std::string name, std::string texture, int z)
: _name(name), _texture(texture), _z(z)
{
}

GameObject::~GameObject()
{
}

void GameObject::Update(float time)
{
  _position += _speed        * time;
  _speed    += _acceleration * time;

  if     (_position.X() > 1.f)
    _position.X() -= 1.f;
  else if(_position.X() < 0.f)
    _position.X() += 1.f;

  if     (_position.Y() > 1.f)
    _position.Y() -= 1.f;
  else if(_position.Y() < 0.f)
    _position.Y() += 1.f;
}
