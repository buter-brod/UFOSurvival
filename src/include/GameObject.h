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
  GameObject(IDType id, std::string texture, unsigned int lifes = 0);
  
  bool operator==(const GameObject& obj) const;
  bool operator!=(const GameObject& obj) const;

  void Update(float time);
  void Destroy(bool instant = false);
  
  float GetDestroyProgress() const;
  bool IsDestroyed() {return _destroyed;}

  Point       GetSize    () const { return _size;     }
  Point       GetPosition() const { return _position; }
  Point       GetSpeed   () const { return _speed;    }

  std::string GetTexture () const { return _texture;  }
  IDType      GetID      () const { return _id;       }

  const VArr& GetVArray  () const {return _vertArray; }
  
  void SetPosition    (Point ps) { _position     =  ps; }
  void SetSize        (Point sz) { _size         =  sz; }
  void SetSpeed       (Point sp) { _speed        =  sp; }
  void SetAcceleration(Point ac) { _acceleration =  ac; }

  void SetMaxLifeTime (float tm) { _maxLifeTime = tm;   }

  unsigned int GetLifes() {return _lifes;}
  
protected:
  Point _size;
  Point _position;
  Point _speed;
  Point _acceleration;

  unsigned int _lifes;

  std::string _texture;
  IDType _id;
  bool  _destroyed;

  float _destroyedSAgo;
  float _lifeTime;
  float _maxLifeTime;
  
  VArr _vertArray;
};

#endif