#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Utils.h"
#include <memory>
#include <list>

#define DECLARE_PTR(TYPENAME) typedef std::shared_ptr          <TYPENAME>   TYPENAME##Ptr
#define DECLARE_LST(TYPENAME) typedef std::list<std::shared_ptr<TYPENAME> > TYPENAME##List

typedef size_t IDType;

class GameObject
{
public:
  GameObject(IDType id, std::string texture);
  virtual ~GameObject(){}
  
  bool operator==(const GameObject& obj) const;
  bool operator!=(const GameObject& obj) const;

  virtual void Update(float time);
  virtual void Destroy(bool instant = false);
  
  float GetDestroyProgress() const;
  bool IsDestroyed() const {return _destroyed;}

  Point       GetSize    () const { return _size;     }
  Point       GetPosition() const { return _position; }
  
  std::string GetTexture () const { return _texture;  }
  IDType      GetID      () const { return _id;       }

  float GetOpacity() const {return _opacity;}

  void SetPosition    (Point ps) { _position     =  ps; }
  void SetSize        (Point sz) { _size         =  sz; }
  
protected:
  Point _size;
  Point _position;
  std::string _texture;
  IDType _id;
  bool  _destroyed;
  float _destroyedSAgo;
  float _opacity;
};

DECLARE_PTR(GameObject);

#endif