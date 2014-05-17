#ifndef _ASTEROID_H_
#define _ASTEROID_H_

#include "GameObject.h"

class Asteroid : public GameObject
{
public:
  Asteroid(IDType id, std::string texture, VArr vertexArr = VArr(), unsigned int lifes = 1);

protected:
  void generatePoly();
};


#endif