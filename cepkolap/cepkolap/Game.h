#ifndef _GAME_H_
#define _GAME_H_

#include <map>
#include <time.h>
#include "GameObject.h"

class Game
{
public:
  Game();

  std::multimap<int, GameObject>& Objects() { return _objects; }
  void Update();
  void SetRatio(float r);
  void InitObjects();

  void EngineFly(Point pos);
  void StopEngine();
  void Shoot(Point at);
  
protected:

  void addObject(GameObject &obj);

  // repair X of current point with current ratio
  Point scpt(Point p);

  std::multimap<int, GameObject> _objects;
  GameObject *_hero;
  
  float _ratio;

  clock_t _timeLastUpdate;
};

#endif