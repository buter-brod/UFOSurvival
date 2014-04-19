#ifndef _ASTEROID_H_
#define _ASTEROID_H_

#include "GameObject.h"

class Asteroid : public GameObject
{
public:
  Asteroid(std::string name);
  ~Asteroid();

protected:
  void generatePoly();
};


#endif