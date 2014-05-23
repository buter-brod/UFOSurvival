#ifndef _GAME_H_
#define _GAME_H_

#include <time.h>

#include "Asteroid.h"
#include "Bullet.h"

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

  const AsteroidList& GetAsteroids() const {return _asteroids;}
  const BulletList  & GetBullets  () const {return _bullets  ;}

  const MovingObjectPtr GetHeroObject      () const { return _heroObject      ;}
  const GameObjectPtr   GetBackgroundObject() const { return _backgroundObject;}
  const GameObjectPtr   GetGameOverObject  () const { return _gameOverObject  ;}
  const GameObjectPtr   GetBlackObject     () const { return _blackObject     ;}
  const GameObjectPtr   GetPanelObject     () const { return _panelObject     ;}
  
  bool IsGameOver() const {return _heroObject->GetDestroyProgress() >= 1.f;}
  
protected:

  void addAsteroid();
  template<typename ObjListType> void addObject(typename ObjListType::value_type objPtr, ObjListType& objects, bool fixObjSize = true)
  {
    if(fixObjSize)
      fixObjectXSize(objPtr);

    objects.push_back(objPtr);
  }

  // repair X of current point with current ratio
  void fixObjectXSize(GameObjectPtr obj) const;
  Point scpt(Point p) const;

  IDType newID();
  
  void crackAsteroid(AsteroidList& added, const AsteroidPtr& asteroid, CrackSpots& crSp);

  AsteroidList _asteroids;
  BulletList _bullets;

  IDType _nextID;
  
  MovingObjectPtr _heroObject;
  GameObjectPtr _backgroundObject, _gameOverObject, _blackObject, _panelObject;
  
  float _ratio;
  bool _paused;

  clock_t _timeLastUpdate;
};

#endif