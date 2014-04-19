#include "Game.h"
#include "Asteroid.h"
#include <algorithm>

float DELAY = 1.f / FPS;
float ENGINE_POWER = 0.5f;

Game::Game()
{
}

void Game::SetRatio(float r)
{
  _ratio = r;
}

void Game::addObject(GameObject &obj)
{
  obj.SetSize(scpt(obj.GetSize()));
  _objects.insert(std::pair<int, GameObject>(obj.GetZ(), obj));
}

void Game::InitObjects()
{
  GameObject bg  ("background", "sky.png",      0);
  GameObject hero("hero",       "spaceowl.png", 1);

  hero.SetSize    (Point(0.125f, 0.075f));
  bg  .SetSize    (Point(_ratio, 1.0f));
  hero.SetPosition(Point(0.8f, 0.8f));
  bg  .SetPosition(Point(0.5f, 0.5f));
  
  Asteroid as1("1");
  as1.SetSize(Point(0.15f, 0.15f));
  as1.SetPosition(Point(0.2f, 0.2f));
  as1.SetSpeed(Point(0.04f, 0.04f));

  addObject(hero);
  addObject(bg);

  addObject(as1);

  _hero = &(++_objects.begin())->second;

  //_hero = &std::find_if(_objects.begin(), _objects.end(), [hero](std::pair<int, GameObject> &o) -> bool {return o.second.GetName() == hero.GetName();})->second;

  {
    //GameObject border = GameObject("border", Point(1.0f, 1.0f), "border.png", 1);
    //border.SetPosition(Point(0.5f, 0.5f));
    //addObject(border);
  }
}

void Game::Update()
{
  clock_t updateStartTime = clock();
  float elapsed = dt(updateStartTime, _timeLastUpdate);
  if(elapsed >= DELAY)
  {
    _timeLastUpdate = updateStartTime;

    for(auto &obj : _objects)
      obj.second.Update(elapsed);
  }
}

void Game::Shoot(Point at)
{
}

Point Game::scpt(Point p)
{
  return Point(p.X() / _ratio, p.Y());
}

void Game::EngineFly(Point pos)
{
  Point moveVec = (pos - _hero->GetPosition()).normalized();
  moveVec *= ENGINE_POWER;
  _hero->SetAcceleration(moveVec);
}

void Game::StopEngine()
{
  _hero->SetAcceleration(Point(0.f, 0.f));
}