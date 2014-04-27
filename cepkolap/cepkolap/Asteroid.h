#ifndef _ASTEROID_H_
#define _ASTEROID_H_

#include "GameObject.h"

class Asteroid : public GameObject
{
public:
  Asteroid(IDType id, std::string texture);

protected:
  void generatePoly();

  bool _cracked = false; // means it's a part of some larger asteroid that has been shot already
};


#endif