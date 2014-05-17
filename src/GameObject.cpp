#include "GameObject.h"
#include <algorithm>

const float DESTROY_TIME = 0.1f;

GameObject::GameObject(IDType id, std::string texture, unsigned int lifes)
: _id(id), _texture(texture), _lifes(lifes), _destroyed(false), _destroyedSAgo(-1.f), _lifeTime(0.f), _maxLifeTime(-1.f)
{
}

void GameObject::Update(float time)
{
  bool flyingFromOutside = 
    _position.X() < 0.f ||
    _position.X() > 1.f ||
    _position.Y() < 0.f ||
    _position.Y() > 1.f;

  _position += _speed        * time;
  _speed    += _acceleration * time;

  if (!flyingFromOutside)
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

  if (_destroyed)
    _destroyedSAgo += time;

  _lifeTime += time;

  if(_maxLifeTime > 0.f && _lifeTime > _maxLifeTime)
    Destroy();
}

bool GameObject::operator==(const GameObject& obj) const
{
  return _id == obj.GetID();
}

bool GameObject::operator!=(const GameObject& obj) const
{
  return !(*this == obj);
}

float GameObject::GetDestroyProgress() const
{
  if (!_destroyed)
    return 0.f;

  return std::min<float>(1.f, _destroyedSAgo / DESTROY_TIME);
}

void GameObject::Destroy(bool instant)
{
  if (!_destroyed)
  {
    _destroyed = true;
    _destroyedSAgo = 0.f;
  }

  if(instant)
    _destroyedSAgo = DESTROY_TIME;
}
