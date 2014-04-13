#include "GameObject.h"


GameObject::GameObject(std::string name, Point size, std::string texture, Point position, int z)
: _name(name), _size(size), _texture(texture), _position(position), _z(z)
{
}

GameObject::GameObject()
{
  throw;
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
