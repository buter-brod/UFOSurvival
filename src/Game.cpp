#include "Game.h"
#include "Asteroid.h"

const float DELAY = 1.f / FPS;

const float ENGINE_POWER    = 0.5f;
const float BULLET_SPEED    = 0.5f;
const float BULLET_LIFETIME = 1.5f;

const unsigned int ASTEROIDS_MIN = 4;

const Point BULLET_SIZE   (0.02f,  0.02f);
const Point HERO_SIZE     (0.125f, 0.075f);
const Point GAMEOVER_SIZE (0.7f,   0.334f);
const Point CENTER        (0.5f,   0.5f);

const std::string TEXTURE_SKY       = "sky.png";
const std::string TEXTURE_PANEL     = "panel.png";
const std::string TEXTURE_HERO      = "spaceowl.png";
const std::string TEXTURE_GAMEOVER1 = "gameover1.png";
const std::string TEXTURE_GAMEOVER2 = "gameover2.png";
const std::string TEXTURE_BLACK     = "black.png";
const std::string TEXTURE_ASTEROID  = "asteroid.png";
const std::string TEXTURE_BULLET    = "bullet.png";

bool CollidesPoly(GameObject &obj1, GameObject &poly, CrackSpots &crsp);

Game::Game()
: _backgroundObject(newID(), TEXTURE_SKY),
  _heroObject      (newID(), TEXTURE_HERO),
  _blackObject     (newID(), TEXTURE_BLACK),
  _panelObject     (newID(), TEXTURE_PANEL),
  _gameOverObject  (newID(), (rnd0xi(2) == 0) ? TEXTURE_GAMEOVER1 : TEXTURE_GAMEOVER2),
  _nextID(0), 
  _paused(false)
{
}

void Game::SetRatio(float r)
{
  _ratio = r;
}

void Game::fixObjectXSize(GameObject &obj)
{
  obj.SetSize(scpt(obj.GetSize()));
}

Point Game::scpt(Point p)
{
  return Point(p.X() / _ratio, p.Y());
}

void Game::addObject(GameObject &obj, ObjectList& objects, bool fixObjSize)
{
  if (fixObjSize)
    fixObjectXSize(obj);

  objects.push_back(obj);
}

void Game::InitObjects()
{
  {
    _gameOverObject  .SetSize(GAMEOVER_SIZE);
    _backgroundObject.SetSize(Point(_ratio, 1.0f));
    _blackObject     .SetSize(Point(_ratio, 1.0f));
    _panelObject     .SetSize(Point(_ratio, 1.0f));
    
    _backgroundObject.SetPosition(CENTER);
    _gameOverObject  .SetPosition(CENTER);
    _blackObject     .SetPosition(CENTER);
    _panelObject     .SetPosition(CENTER);
  }
  {
    fixObjectXSize(_backgroundObject);
    fixObjectXSize(_gameOverObject  );
    fixObjectXSize(_blackObject     );
    fixObjectXSize(_panelObject     );
  }

  Start();
}

void checkDestroyedObjects(ObjectList& objects)
{
  objects.remove_if([](const GameObject &o) {return o.GetDestroyProgress() >= 1.f;});
}

void makeCrackedPoly(VArr inputVArr, CrackSpots crSp, unsigned int first, VArr &arr)
{
  unsigned int second = (first + 1) % 2;

  arr.push_back(crSp[first]._p);

  for(unsigned int i = crSp[first]._i; i != crSp[second]._i;)
  {
    i = (i + 1) % inputVArr.size();
    arr.push_back(inputVArr[i]);
  }
  arr.push_back(crSp[second]._p);
}

void Game::crackAsteroid(ObjectList& added, GameObject& asteroid, CrackSpots& crSp)
{
  VArr newArrs[2];
  const VArr& arr = asteroid.GetVArray();

  Point crackSegmentVec = crSp[0]._p - crSp[1]._p;
  Point orthoSpeed = Point(- crackSegmentVec.Y(), crackSegmentVec.X()).normalized() * asteroid.GetSpeed().len() / 2.f;
  // getting perpendicular vec to speed vec (crack line) to make pieces fly away from each other; using crackSegmentVec rotated to PI/2
    
  // here is the trick: for new polygon, we take first cracking point, second cracking point and all the vertices that are between them, CCW. 
  // For 2nd new polygon we do the same, but in backward direction
  makeCrackedPoly(arr, crSp, 0, newArrs[0]);
  makeCrackedPoly(arr, crSp, 1, newArrs[1]);

  Asteroid newAsteroids[2] = {Asteroid(newID(), TEXTURE_ASTEROID, newArrs[0], 0),
                              Asteroid(newID(), TEXTURE_ASTEROID, newArrs[1], 0)
  // last parameter sets lives to 0 (default for new asteroids is 1), so cracked parts won't be parted again when hit
  };
  newAsteroids[0].SetPosition(asteroid.GetPosition());
  newAsteroids[1].SetPosition(asteroid.GetPosition());

  newAsteroids[0].SetSize(asteroid.GetSize());
  newAsteroids[1].SetSize(asteroid.GetSize());

  newAsteroids[0].SetSpeed(asteroid.GetSpeed() + orthoSpeed);
  newAsteroids[1].SetSpeed(asteroid.GetSpeed() - orthoSpeed);

  added.push_back(newAsteroids[0]);
  added.push_back(newAsteroids[1]);
}

void Game::Update()
{
  if (_paused)
    return;

  clock_t updateStartTime = clock();
  float elapsed = dt(updateStartTime, _timeLastUpdate);
  if(elapsed >= DELAY && !IsGameOver())
  {
    _timeLastUpdate = updateStartTime;

    ObjectList newAsteroids; // for deferred adding of new asteroids (that are parts of cracked one)

    // find objects that are not visible anymore & need to be actually removed from scene
    checkDestroyedObjects(GetAsteroids());
    checkDestroyedObjects(GetBullets());
        
    _heroObject.Update(elapsed);

    for(GameObject& bullet : _bullets)
      bullet.Update(elapsed);

    for(GameObject& asteroid : _asteroids)
    {
      asteroid.Update(elapsed);

      CrackSpots crSpUnused; // just for correct syntax, no need to get the crack line here

      if(!asteroid.IsDestroyed() && CollidesPoly(_heroObject, asteroid, crSpUnused))
        _heroObject.Destroy();

      for (GameObject& bullet : _bullets) //check if bullet hits asteroid
      {
        CrackSpots crSp;
        if(!asteroid.IsDestroyed() && 
           !bullet  .IsDestroyed() && 
           CollidesPoly(bullet, asteroid, crSp))
        {
          if (asteroid.GetLifes() > 0)
          {
            crackAsteroid(newAsteroids, asteroid, crSp);
            asteroid.Destroy(true); // immediately remove 'parent'-asteroid from scene without any animation
          }
          else
            asteroid.Destroy(); // this is already cracked asteroid, so just destroy it in usual way

          bullet.Destroy();
        }
      }
    }
    while (_asteroids.size() < ASTEROIDS_MIN)
    {
      Asteroid newAsteroid(newID(), TEXTURE_ASTEROID);
      addObject(newAsteroid, GetAsteroids());
    }

    for (GameObject &obj : newAsteroids)
      addObject(obj, GetAsteroids(), false); //last parameter is false here (default = true) 'cause we're reusing old Size that is already adapted for world aspect ratio
  }
}

void Game::Shoot(Point at)
{
  GameObject bullet(newID(), TEXTURE_BULLET);
  bullet.SetPosition(_heroObject.GetPosition());
  bullet.SetSpeed((at - bullet.GetPosition()).normalized() * BULLET_SPEED);
  bullet.SetSize(BULLET_SIZE);
  bullet.SetMaxLifeTime(BULLET_LIFETIME);
  addObject(bullet, GetBullets());
}

void Game::EngineFly(Point pos)
{
  _heroObject.SetAcceleration((pos - _heroObject.GetPosition()).normalized() * ENGINE_POWER);
}

void Game::EngineStop()
{
  _heroObject.SetAcceleration(Point(0.f, 0.f));
}

IDType Game::newID()
{
  return _nextID++;
}

void Game::Start()
{
  _asteroids.clear();
  _bullets  .clear();

  if (IsGameOver()) //restart
    _heroObject = GameObject(newID(), TEXTURE_HERO);
  
  _heroObject.SetPosition(CENTER);
  _heroObject.SetSize    (HERO_SIZE);
  
  fixObjectXSize(_heroObject);
}

void Game::Pause()
{
  Pause(!_paused);
}

void Game::Pause(bool pause)
{
  if (!pause)
    _timeLastUpdate = clock();

  _paused = pause;
}
