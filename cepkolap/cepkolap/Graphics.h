#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <glut.h>
#include <map>
#include <time.h>
#include "Game.h"

class Graphics
{
public:

  void LoadTexture(std::string texName);  
  void Frame();
  void Init(Game *game, Point size);

protected:
  
  void initGL();
  void loadTextures();
  void frame();
  void drawObject(GameObject& obj);
  void draw(GLuint tInd, Point pos, Point sz);
  GLuint loadTexture(std::string texName);
  Point posToScreen (Point p);
  Point sizeToScreen(Point p);
  Point _size;
  std::map<std::string, GLuint> _textureMap;

  Game *_game;
  
  clock_t _timeLastRedraw;
};

#endif