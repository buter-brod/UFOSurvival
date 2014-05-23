#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "glHeaders.h"
#include <map>
#include <time.h>

#include "Game.h"

class Graphics
{
public:

  void LoadTexture(std::string texName);  
  bool Frame();
  void Init(Game *game, Point size);
  void LoadVertex(IDType id, const VArr &vVec);
  ~Graphics();

protected:

  struct VBO
  {
    VBO(GLuint v = 0, GLuint t = 0): _v(v), _t(t) {}
    GLuint _v, _t;
  };

  void delVBO(Graphics::VBO &vbo);
  void cleanup();
  void initGL();
  inline float ratio() {return _size.X() / _size.Y();}
  void loadVertex(GLvoid *vvp, size_t vvSize, GLvoid *uvp, size_t uvSize, IDType id);
  void loadObjectData();
  void loadTexture(const GameObjectPtr& obj);
  void loadVertex (const AsteroidPtr  & ast);

  template<typename ObjListType> void loadTextures(const ObjListType& objects)
  {
    for(const typename ObjListType::value_type &obj : objects)
      loadTexture(obj);
  }
  
  template<typename ObjListType> void drawObjects(const ObjListType& objects)
  {
    for(const typename ObjListType::value_type& obj : objects)
      drawObject(obj);
  }
  
  void frame();

  void drawObject (const GameObjectPtr& obj, bool defaultRectangle = true, size_t vCount = 4);
  void drawObject (const AsteroidPtr& ast);
  void draw(GLuint tInd, GLuint vBuf, GLuint tBuf, size_t fCount, Point pos, Point sz, float opacity);
  Point posToScreen (Point p);
  Point sizeToScreen(Point p);
  Point _size;
  std::map<std::string, GLuint> _textureMap;

  struct VBOData
  {
    VBOData() :_initWithDefault(false) {}

    std::map<IDType, VBO> _vboMap;
    IDType _defaultVBOID;
    bool _initWithDefault;
  };

  VBOData _vboData;
  const Game *_game;
  clock_t _timeLastRedraw;
};

#endif
