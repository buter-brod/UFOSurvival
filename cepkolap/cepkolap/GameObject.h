#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Utils.h"

class GameObject
{
public:
  GameObject(std::string name, Point size, std::string texture, Point position, int z);
  GameObject();
  ~GameObject();

  void Update(float time);

  Point       GetSize    () { return _size;     }
  Point       GetPosition() { return _position; }
  Point       GetSpeed   () { return _speed;    }
  std::string GetTexture () { return _texture;  }
  std::string GetName    () { return _name;     }
  int         GetZ       () { return _z;        }

  void SetPosition    (Point ps) { _position     =  ps; }
  void SetSize        (Point sz) { _size         =  sz; }
  void SetSpeed       (Point sp) { _speed        =  sp; }
  void SetAcceleration(Point ac) { _acceleration =  ac; }
  
protected:
  Point _size;
  Point _position;
  Point _speed;
  Point _acceleration;
  int _z;
  std::string _texture;
  std::string _name;
};

#endif