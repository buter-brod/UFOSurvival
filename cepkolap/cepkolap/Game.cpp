#include "Game.h"
#include "Asteroid.h"
#include "Collider.h"
#include <algorithm>

static const float DELAY = 1.f / FPS;

static const float ENGINE_POWER    = 0.5f;
static const float BULLET_SPEED    = 0.5f;
static const float BULLET_LIFETIME = 1.5f;

static const unsigned int ASTEROIDS_MIN = 4;

static const Point BULLET_SIZE   (0.02f,  0.02f);
static const Point HERO_SIZE     (0.125f, 0.075f);
static const Point GAMEOVER_SIZE (0.7f,   0.334f);
static const Point CENTER        (0.5f,   0.5f);

static const std::string TEXTURE_SKY       = "sky.png";
static const std::string TEXTURE_HERO      = "spaceowl.png";
static const std::string TEXTURE_GAMEOVER1 = "gameover1.png";
static const std::string TEXTURE_GAMEOVER2 = "gameover2.png";
static const std::string TEXTURE_BLACK     = "black.png";
static const std::string TEXTURE_ASTEROID  = "asteroid.png";
static const std::string TEXTURE_BULLET    = "bullet.png";

Game::Game()
: _backgroundObject(newID(), TEXTURE_SKY),
  _heroObject      (newID(), TEXTURE_HERO),
  _blackObject     (newID(), TEXTURE_BLACK),
  _gameOverObject  (newID(), (rnd0xi(2) == 0) ? TEXTURE_GAMEOVER1 : TEXTURE_GAMEOVER2)
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

void Game::addObject(GameObject &obj, ObjectList& objects)
{
  fixObjectXSize   (obj);
  objects.push_back(obj);
}

void Game::InitObjects()
{
  {
    _heroObject      .SetSize(HERO_SIZE);
    _gameOverObject  .SetSize(GAMEOVER_SIZE);
    _backgroundObject.SetSize(Point(_ratio, 1.0f));
    _blackObject     .SetSize(Point(_ratio, 1.0f));
    
    _backgroundObject.SetPosition(CENTER);
    _heroObject      .SetPosition(CENTER);
    _gameOverObject  .SetPosition(CENTER);
    _blackObject     .SetPosition(CENTER);
  }
  {
    fixObjectXSize(_backgroundObject);
    fixObjectXSize(_heroObject      );
    fixObjectXSize(_gameOverObject  );
    fixObjectXSize(_blackObject     );
  }
   
  //_hero = &std::find_if(_objects.begin(), _objects.end(), [hero](std::pair<int, GameObject> &o) -> bool {return o.second.GetName() == hero.GetName();})->second;

  {
    //GameObject border = GameObject("border", Point(1.0f, 1.0f), "border.png", 1);
    //border.SetPosition(Point(0.5f, 0.5f));
    //addObject(border);
  }
}

void checkDestroyedObjects(ObjectList& objects)
{
  objects.remove_if([](const GameObject &o) {return o.GetDestroyProgress() >= 1.f;});
}

void Game::Update()
{
  clock_t updateStartTime = clock();
  float elapsed = dt(updateStartTime, _timeLastUpdate);
  if(elapsed >= DELAY && !IsGameOver())
  {
    _timeLastUpdate = updateStartTime;

    checkDestroyedObjects(GetAsteroids());
    checkDestroyedObjects(GetBullets());
        
    _heroObject.Update(elapsed);

    for(GameObject& bullet : _bullets)
      bullet.Update(elapsed);

    for(GameObject& asteroid : _asteroids)
    {
      asteroid.Update(elapsed);

      if(!asteroid.IsDestroyed() && Collider::Collides(_heroObject, asteroid, Point(), Point())) //GOD MODE :D
        _heroObject.Destroy();  

      for (GameObject& bullet : _bullets)
      {
        Point coll1, coll2;
        if(!asteroid.IsDestroyed() && 
           !bullet  .IsDestroyed() && 
           Collider::Collides(bullet, asteroid, coll1, coll2))
        {
          asteroid.Destroy();
          bullet  .Destroy();
        }
      }
    }
    while (_asteroids.size() < ASTEROIDS_MIN)
      addObject(Asteroid(newID(), TEXTURE_ASTEROID), GetAsteroids());
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
  return nextID++;
}
