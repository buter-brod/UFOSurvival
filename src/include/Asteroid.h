#ifndef _ASTEROID_H_
#define _ASTEROID_H_

#include "MovingObject.h"
#include <vector>

typedef std::vector<Point> VArr;

class Asteroid : public MovingObject
{
public:
  Asteroid(IDType id, std::string texture, size_t lifes = 1u);

  void SetVArray(const VArr& vArr) {_vertArray = vArr;}

  const VArr& GetVArray();
  size_t GetLifes() {return _lifes;}

protected:
  void generatePoly();

  size_t _lifes;
  VArr _vertArray;
};

DECLARE_PTR(Asteroid);
DECLARE_LST(Asteroid);

#endif