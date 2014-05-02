#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Utils.h"
#include <vector>

typedef unsigned long IDType;
// sufficent for 40 days creating one ID per millisecond

typedef std::vector<Point> VArr;

class GameObject
{
public:
  GameObject(IDType id, std::string texture);
  
  bool operator==(const GameObject& obj) const;
  bool operator!=(const GameObject& obj) const;

  void Update(float time);
  void Destroy();
  
  float GetDestroyProgress() const;
  bool IsDestroyed() {return _destroyed;}

  Point       GetSize    () const { return _size;     }
  Point       GetPosition() const { return _position; }
  Point       GetSpeed   () const { return _speed;    }

  std::string GetTexture () const { return _texture;  }
  IDType      GetID      () const { return _id;       }

  VArr& GetVArray() {return _vertArray;}

  void SetPosition    (Point ps) { _position     =  ps; }
  void SetSize        (Point sz) { _size         =  sz; }
  void SetSpeed       (Point sp) { _speed        =  sp; }
  void SetAcceleration(Point ac) { _acceleration =  ac; }

  void SetMaxLifeTime (float tm) { _maxLifeTime = tm;   }
  
protected:
  Point _size;
  Point _position;
  Point _speed;
  Point _acceleration;

  std::string _texture;
  IDType _id;
  bool  _destroyed = false;

  float _destroyedSAgo = -1.f;
  float _lifeTime = 0.f;
  float _maxLifeTime = -1.f;
  
  VArr _vertArray;
};

#endif