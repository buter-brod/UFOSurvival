#include "GameObject.h"
#include <algorithm>

const float DESTROY_TIME = 0.1f;

GameObject::GameObject(IDType id, std::string texture)
: _id(id), _texture(texture), _destroyed(false), _destroyedSAgo(-1.f), _opacity(1.f)
{
}

void GameObject::Update(float time)
{
  if (_destroyed)
    _destroyedSAgo += time;

  _opacity = 1.f - GetDestroyProgress();
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
