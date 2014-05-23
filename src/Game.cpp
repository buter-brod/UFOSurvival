#include "Game.h"

const float DELAY = 1.f / FPS;

const float ENGINE_POWER    = 0.5f;
const float BULLET_SPEED    = 0.5f;
const float BULLET_LIFETIME = 1.5f;

const size_t ASTEROIDS_MIN = 4;

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

bool CollidesPoly(MovingObjectPtr obj1, AsteroidPtr &poly, CrackSpots &crsp);

Game::Game()
: _nextID(0), 
  _paused(false)
{
}

void Game::SetRatio(float r)
{
  _ratio = r;
}

void Game::fixObjectXSize(GameObjectPtr obj) const
{
  obj->SetSize(scpt(obj->GetSize()));
}

Point Game::scpt(Point p) const
{
  return Point(p.X() / _ratio, p.Y());
}

void Game::InitObjects()
{
  {
    _heroObject       = std::make_shared<MovingObject>(newID(), TEXTURE_HERO);

    _backgroundObject = std::make_shared<GameObject>  (newID(), TEXTURE_SKY);
    _blackObject      = std::make_shared<GameObject>  (newID(), TEXTURE_BLACK);
    _panelObject      = std::make_shared<GameObject>  (newID(), TEXTURE_PANEL);
    _gameOverObject   = std::make_shared<GameObject>  (newID(), (rnd0xi(2) == 0) ? TEXTURE_GAMEOVER1 : TEXTURE_GAMEOVER2);

    _gameOverObject  ->SetSize(GAMEOVER_SIZE);
    _backgroundObject->SetSize(Point(_ratio, 1.0f));
    _blackObject     ->SetSize(Point(_ratio, 1.0f));
    _panelObject     ->SetSize(Point(_ratio, 1.0f));
    
    _backgroundObject->SetPosition(CENTER);
    _gameOverObject  ->SetPosition(CENTER);
    _blackObject     ->SetPosition(CENTER);
    _panelObject     ->SetPosition(CENTER);
  }
  {
    fixObjectXSize(_backgroundObject);
    fixObjectXSize(_gameOverObject  );
    fixObjectXSize(_blackObject     );
    fixObjectXSize(_panelObject     );
  }

  Start();
}

template<typename ObjListType> void checkDestroyedObjects(ObjListType& objects)
{
  objects.remove_if([](const GameObjectPtr &o) {return o->GetDestroyProgress() >= 1.f;});
}

void makeCrackedPoly(const VArr &inputVArr, CrackSpots crSp, size_t first, VArr &arr)
{
  size_t second = (first + 1) % 2;

  arr.push_back(crSp[first]._p);

  for(size_t i = crSp[first]._i; i != crSp[second]._i;)
  {
    i = (i + 1u) % inputVArr.size();
    arr.push_back(inputVArr[i]);
  }
  arr.push_back(crSp[second]._p);
}

void Game::crackAsteroid(AsteroidList& added, const AsteroidPtr& asteroid, CrackSpots& crSp)
{
  VArr newArrs[2];
  const VArr& arr = asteroid->GetVArray();

  Point crackSegmentVec = crSp[0]._p - crSp[1]._p;
  Point orthoSpeed = Point(- crackSegmentVec.Y(), crackSegmentVec.X()).normalized() * asteroid->GetSpeed().len() / 2.f;
  // getting perpendicular vec to speed vec (crack line) to make pieces fly away from each other; using crackSegmentVec rotated to PI/2
    
  // here is the trick: for new polygon, we take first cracking point, second cracking point and all the vertices that are between them, CCW. 
  // For 2nd new polygon we do the same, but in backward direction
  makeCrackedPoly(arr, crSp, 0, newArrs[0]);
  makeCrackedPoly(arr, crSp, 1, newArrs[1]);

  AsteroidPtr newAsteroids[2] = {std::make_shared<Asteroid>(newID(), TEXTURE_ASTEROID, 0),
                                 std::make_shared<Asteroid>(newID(), TEXTURE_ASTEROID, 0)
  // last parameter sets lives to 0 (default for new asteroids is 1), so cracked parts won't be parted again when hit
  };

  newAsteroids[0]->SetVArray(newArrs[0]);
  newAsteroids[1]->SetVArray(newArrs[1]);

  newAsteroids[0]->SetPosition(asteroid->GetPosition());
  newAsteroids[1]->SetPosition(asteroid->GetPosition());

  newAsteroids[0]->SetSize(asteroid->GetSize());
  newAsteroids[1]->SetSize(asteroid->GetSize());

  newAsteroids[0]->SetSpeed(asteroid->GetSpeed() + orthoSpeed);
  newAsteroids[1]->SetSpeed(asteroid->GetSpeed() - orthoSpeed);

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

    AsteroidList newAsteroids; // for deferred adding of new asteroids (that are parts of cracked one)

    // find objects that are not visible anymore & need to be actually removed from scene
    checkDestroyedObjects(_asteroids);
    checkDestroyedObjects(_bullets);
        
    _heroObject->Update(elapsed);

    for(BulletPtr& bullet : _bullets)
      bullet->Update(elapsed);

    for(AsteroidPtr& asteroid : _asteroids)
    {
      asteroid->Update(elapsed);

      CrackSpots crSpUnused; // just for correct syntax, no need to get the crack line here

      if(!asteroid->IsDestroyed() && CollidesPoly(_heroObject, asteroid, crSpUnused))
        _heroObject->Destroy();

      for (BulletPtr& bullet : _bullets) //check if bullet hits asteroid
      {
        CrackSpots crSp;
        if(!asteroid->IsDestroyed() && 
           !bullet  ->IsDestroyed() && 
           CollidesPoly(bullet, asteroid, crSp))
        {
          if (asteroid->GetLifes() > 0)
          {
            crackAsteroid(newAsteroids, asteroid, crSp);
            asteroid->Destroy(true); // immediately remove 'parent'-asteroid from scene without any animation
          }
          else
            asteroid->Destroy(); // this is already cracked asteroid, so just destroy it in usual way

          bullet->Destroy();
        }
      }
    }
    while (_asteroids.size() < ASTEROIDS_MIN)
      addObject(std::make_shared<Asteroid>(newID(), TEXTURE_ASTEROID), _asteroids);

    for (AsteroidPtr &ast : newAsteroids)
      addObject(ast, _asteroids, false); //last parameter is false here (default = true) 'cause we're reusing old Size that is already adapted for world aspect ratio
  }
}

void Game::Shoot(Point at)
{
  BulletPtr bullet = std::make_shared<Bullet>(newID(), TEXTURE_BULLET);
  bullet->SetPosition(_heroObject->GetPosition());
  bullet->SetSpeed((at - bullet->GetPosition()).normalized() * BULLET_SPEED);
  bullet->SetSize(BULLET_SIZE);
  bullet->SetMaxLifeTime(BULLET_LIFETIME);
  addObject(bullet, _bullets);
}

void Game::EngineFly(Point pos)
{
  _heroObject->SetAcceleration((pos - _heroObject->GetPosition()).normalized() * ENGINE_POWER);
}

void Game::EngineStop()
{
  _heroObject->SetAcceleration(Point(0.f, 0.f));
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
    _heroObject = std::make_shared<MovingObject>(newID(), TEXTURE_HERO);
  
  _heroObject->SetPosition(CENTER);
  _heroObject->SetSize    (HERO_SIZE);
  
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
