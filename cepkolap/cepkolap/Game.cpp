#include "Game.h"
#include "GameObject.h"
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
  GameObject bg    = GameObject("background", Point(_ratio, 1.0f),  "sky.png", Point(0.5f, 0.5f), 0);
  GameObject hero  = GameObject("hero",       Point(0.15f, 0.075f), "UFO.png", Point(0.8f, 0.8f), 1);


  //hero.SetSpeed(Point(-0.3f, 0.f));
  //hero.SetAcceleration(Point(0.1f, -0.01f));

  addObject(hero);
  addObject(bg);  

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