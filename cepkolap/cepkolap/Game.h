#ifndef _GAME_H_
#define _GAME_H_

#include <map>
#include <time.h>
#include "GameObject.h"
#include <list>

typedef std::list<GameObject> ObjectList;

class Game
{
public:
  Game();

  void Update();
  void SetRatio(float r);
  void InitObjects();

  void EngineFly(Point pos);
  void EngineStop();
  void Shoot(Point at);

  ObjectList& GetAsteroids() {return _asteroids;}
  ObjectList& GetBullets()   {return _bullets;}

  GameObject& GetHeroObject()       {return _heroObject;}
  GameObject& GetBackgroundObject() {return _backgroundObject;}
  GameObject& GetGameOverObject()   {return _gameOverObject;}
  GameObject& GetBlackObject()      {return _blackObject;}
  
  bool IsGameOver() {return _heroObject.GetDestroyProgress() >= 1.f;}
  
protected:

  void addAsteroid();
  void addObject(GameObject &as, ObjectList& objects);
    
  // repair X of current point with current ratio
  void fixObjectXSize(GameObject &obj);
  Point scpt(Point p);

  IDType newID();
  
  void checkGameOver();

  ObjectList _asteroids, _bullets;

  IDType nextID = 0;
  
  GameObject _heroObject, _backgroundObject, _gameOverObject, _blackObject;
  
  float _ratio;

  clock_t _timeLastUpdate;
};

#endif