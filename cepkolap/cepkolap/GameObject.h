#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Utils.h"
#include <vector>

class GameObject
{
public:
  GameObject(std::string name, std::string texture, int z);
  ~GameObject();
  
  void Update(float time);

  Point       GetSize    () { return _size;     }
  Point       GetPosition() { return _position; }
  Point       GetSpeed   () { return _speed;    }
  std::string GetTexture () { return _texture;  }
  std::string GetName    () { return _name;     }
  int         GetZ       () { return _z;        }

  std::vector<Point>& GetVertexArray() {return _vertArray;}

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

  std::vector<Point> _vertArray;
};

#endif