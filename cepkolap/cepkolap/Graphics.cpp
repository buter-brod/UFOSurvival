#include "Graphics.h"
#include <FreeImage.h>
#include "Utils.h"
#include "Log.h"

static float DELAY = 1.f / FPS;

#define DEFAULT_VBO "quad"

static GLfloat vertices[4][3] =
{
  {-1.f, -1.f, 0},
  { 1.f, -1.f, 0},
  { 1.f,  1.f, 0},
  {-1.f,  1.f, 0}
};

static GLfloat uvs[4][2] =
{
  {0.f, 0.f},
  {1.f, 0.f},
  {1.f, 1.f},
  {0.f, 1.f}
};

void Graphics::initGL()
{
  glewInit();
  glViewport(0, 0, _size.I_X(), _size.I_Y());

  GLfloat ratio = _size.X() / _size.Y();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-ratio, ratio, -1, 1, 1, 10);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glFrontFace(GL_CW);
  glShadeModel(GL_SMOOTH);
}

void Graphics::loadObjectData(GameObject& obj)
{
  LoadTexture(obj.GetTexture());

  if(!obj.GetVertexArray().empty())
    LoadVertex(obj.GetID(), obj.GetVertexArray());
}

void Graphics::loadObjectData(ObjectList& objects)
{
  for(GameObject& obj : objects)
    loadObjectData(obj);
}

void Graphics::loadObjectData()
{
  loadObjectData(_game->GetAsteroids());
  loadObjectData(_game->GetBullets());
  
  loadObjectData(_game->GetBlackObject());
  loadObjectData(_game->GetBackgroundObject());
  loadObjectData(_game->GetHeroObject());
  loadObjectData(_game->GetGameOverObject());
}

void Graphics::LoadVertex(IDType id, std::vector<Point> &vVec)
{
  std::vector<float> vertexVecFlat, uvVecFlat;

  for(Point p : vVec)
  {
    vertexVecFlat.push_back(p.X());
    vertexVecFlat.push_back(p.Y());
    vertexVecFlat.push_back(0.f);

    uvVecFlat.push_back((p.X() + 1) / 2);
    uvVecFlat.push_back((p.Y() + 1) / 2);
  }
  loadVertex(vertexVecFlat.data(), vertexVecFlat.size(), uvVecFlat.data(), uvVecFlat.size(), id);
}

void Graphics::loadVertex(GLvoid *vvp, unsigned int vvSize, GLvoid *uvp, unsigned int uvSize, IDType id)
{
  if(_vboData._vboMap.count(id) > 0)
  {
    Log::Print("VBO " + Utils::ToString(id) + " already exists with ids=" + Utils::ToString(_vboData._vboMap[id]._t) + "," + Utils::ToString(_vboData._vboMap[id]._v));
    return;
  }

  GLuint bufN[2];
  glGenBuffers(2, bufN);
  glBindBuffer(GL_ARRAY_BUFFER, bufN[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vvSize, vvp, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, bufN[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uvSize, uvp, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  _vboData._vboMap[id] = VBO(bufN[0], bufN[1]);
}

void Graphics::Init(Game *game, Point size)
{
  _game = game;
  _size = size;

  initGL();
  loadObjectData();
}

GLuint Graphics::loadTexture(std::string texName)
{
  GLuint texInd = -1;

  FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(texName.c_str(), 0), texName.c_str());

  unsigned int w = FreeImage_GetWidth(bitmap);
  unsigned int h = FreeImage_GetHeight(bitmap);

  glGenTextures(1, &texInd);

  glBindTexture(GL_TEXTURE_2D, texInd);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h,
    0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(bitmap));

  FreeImage_Unload(bitmap);
  return texInd;
}

void Graphics::LoadTexture(std::string texName)
{
  if(_textureMap.count(texName) > 0)
  {
    Log::Print("Texture " + texName + " already exists with id=" + Utils::ToString(_textureMap[texName]));
    return;
  }
  _textureMap[texName] = loadTexture(texName);
}

std::vector<GLubyte> glUBrange(unsigned int r)
{
  std::vector<GLubyte> vec;
  for(unsigned int i = 0; i < r; i++)
    vec.push_back(i);
  return vec;
}

void Graphics::draw(GLuint tInd, GLuint vBuf, GLuint tBuf, unsigned int vCount, Point pos, Point size, float opacity)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
   
  glTranslatef( pos.X(),  pos.Y(), -1.0f);
  glScalef    (size.X(), size.Y(),  1.f);

  glColor4f(1.0, 1.0, 1.0, opacity);

  glBindTexture(GL_TEXTURE_2D, tInd);

  glBindBuffer(GL_ARRAY_BUFFER, vBuf);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, tBuf);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, 0, 0);

  std::vector<GLubyte>& indV = glUBrange(vCount);
  
  glDrawArrays(GL_TRIANGLE_FAN, 0, vCount);
}

Point Graphics::posToScreen(Point p)
{
  return Point(
    (2.f * p.X() - 1.f) * (_size.X() / _size.Y()),
    (2.f * p.Y() - 1.f));
}

Point Graphics::sizeToScreen(Point p)
{
  return Point(
    p.X() * (_size.X() / _size.Y()),
    p.Y());
}

void Graphics::drawObjects(ObjectList& objects)
{
  for (GameObject& obj : objects)
    drawObject(obj);
}

void Graphics::drawObject(GameObject& obj)
{
  Point posScreen =  posToScreen(obj.GetPosition());
  Point sizScreen = sizeToScreen(obj.GetSize());

  unsigned int vCount = obj.GetVertexArray().size();

  if(_textureMap.count(obj.GetTexture()) < 1)
    LoadTexture(obj.GetTexture());

  IDType vboID = obj.GetID();

  if(vCount > 0)
  {
    if(_vboData._vboMap.count(obj.GetID()) < 1)
      LoadVertex(obj.GetID(), obj.GetVertexArray()); // object looks not pre-initialized, so let's load vertex data right now.
  }
  else
  {
    if (!_vboData.initWithDefault)
    {
      _vboData._defaultVBOID = obj.GetID();
      loadVertex(vertices, 4*3, uvs, 4*2, _vboData._defaultVBOID);
      _vboData.initWithDefault = true;
    }
    else
      vboID = _vboData._defaultVBOID; // object has no its own vertex data, so we'll use default -1..1 rectangle

    vCount  = 4;
  }

  GLuint tex = _textureMap[obj.GetTexture()];
  VBO vbo = _vboData._vboMap[vboID];
  float opacity = 1.f - obj.GetDestroyProgress();

  draw(tex, vbo._v, vbo._t, vCount, posScreen, sizScreen, opacity);

  /*{
    float x = obj.GetPosition().X();
    float y = obj.GetPosition().Y();

    float xs2 = obj.GetSize().X() / 2.f;
    float ys2 = obj.GetSize().Y() / 2.f;

    if(x > 1.f - xs2)
      draw(tex, vbo._v, vbo._t, vCount, posToScreen(Point(x - 1.f, y)), sizScreen, opacity);

    if(x < 0.f + xs2)
      draw(tex, vbo._v, vbo._t, vCount, posToScreen(Point(x + 1.f, y)), sizScreen, opacity);

    if(y > 1.f - ys2)
      draw(tex, vbo._v, vbo._t, vCount, posToScreen(Point(x, y - 1.f)), sizScreen, opacity);

    if(y < 0.f + ys2)
      draw(tex, vbo._v, vbo._t, vCount, posToScreen(Point(x, y + 1.f)), sizScreen, opacity);
    
  }*/
}

void Graphics::frame()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  {
    drawObject(_game->GetBackgroundObject());

    if(!_game->IsGameOver())
    {
      drawObject(_game->GetHeroObject());

      drawObjects(_game->GetAsteroids());
      drawObjects(_game->GetBullets());
    }
    else
    {
      drawObject(_game->GetBlackObject());
      drawObject(_game->GetGameOverObject());
    }
  }

  glFlush();
}

void Graphics::Frame()
{
  clock_t frameStartTime = clock();
  if(dt(frameStartTime, _timeLastRedraw) >= DELAY)
  {
    _timeLastRedraw = frameStartTime;

    frame();
  }
}
