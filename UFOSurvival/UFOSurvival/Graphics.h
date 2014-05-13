#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <glew.h>
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
  void LoadVertex(IDType id, VArr &vVec);

protected:
  
  void initGL();
  void loadVertex(GLvoid *vvp, unsigned int vvSize, GLvoid *uvp, unsigned int uvSize, IDType id);
  void loadObjectData();
  void loadObjectData(GameObject& obj);
  void loadObjectData(ObjectList& objects);
  void frame();
  void drawObjects(ObjectList& objects);
  void drawObject (GameObject& obj);
  void draw(GLuint tInd, GLuint vBuf, GLuint tBuf, unsigned int fCount, Point pos, Point sz, float opacity);
  GLuint loadTexture(std::string texName);
  Point posToScreen (Point p);
  Point sizeToScreen(Point p);
  Point _size;
  std::map<std::string, GLuint> _textureMap;

  struct VBO
  {
    VBO(GLuint v = 0, GLuint t = 0) : _v(v), _t(t) {}
    GLuint _v, _t;
  };

  struct VBOData
  {
    std::map<IDType, VBO> _vboMap;
    IDType _defaultVBOID;
    bool initWithDefault = false;
  };

  VBOData _vboData;
  Game *_game;
  clock_t _timeLastRedraw;
};

#endif