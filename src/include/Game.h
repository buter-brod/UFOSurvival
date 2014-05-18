#ifndef _GAME_H_
#define _GAME_H_

#include <time.h>
#include <list>

#include "GameObject.h"

typedef std::list<GameObject> ObjectList;

class Game
{
public:
  Game();

  void Update();

  void SetRatio(float r);
  void InitObjects();
  void Start();
  void Pause(bool pause);
  void Pause(); // toggle

  void EngineFly(Point pos);
  void EngineStop();
  void Shoot(Point at);

  ObjectList& GetAsteroids() {return _asteroids;}
  ObjectList& GetBullets()   {return _bullets;}

  GameObject& GetHeroObject()       {return _heroObject;}
  GameObject& GetBackgroundObject() {return _backgroundObject;}
  GameObject& GetGameOverObject()   {return _gameOverObject;}
  GameObject& GetBlackObject()      {return _blackObject;}
  GameObject& GetPanelObject()      {return _panelObject;}
  
  bool IsGameOver() {return _heroObject.GetDestroyProgress() >= 1.f;}
  
protected:

  void addAsteroid();
  void addObject(GameObject &as, ObjectList& objects, bool fixObjSize = true);
    
  // repair X of current point with current ratio
  void fixObjectXSize(GameObject &obj);
  Point scpt(Point p);

  IDType newID();
  
  void checkGameOver();
  void crackAsteroid(ObjectList& added, GameObject& asteroid, GameObject& bullet, CrackSpots& crSp);
  ObjectList _asteroids, _bullets;

  IDType _nextID;
  
  GameObject _heroObject, _backgroundObject, _gameOverObject, _blackObject, _panelObject;
  
  float _ratio;
  bool _paused;

  clock_t _timeLastUpdate;
};

#endif