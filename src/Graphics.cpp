#include "Graphics.h"

#include "Log.h"
#include "loadTexture.h"

static float DELAY = 1.f / FPS;

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
  glViewport(0, 0, _size.I_X(), _size.I_Y());

  GLfloat ratio = _size.X() / _size.Y();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-ratio, ratio, -1, 1, 1, 10);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

  if(!obj.GetVArray().empty())
    LoadVertex(obj.GetID(), obj.GetVArray());
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

void Graphics::LoadVertex(IDType id, const VArr &vVec)
{
  std::vector<float> vertexVecFlat, uvVecFlat;

  for(Point p : vVec)
  {
    vertexVecFlat.push_back(p.X());
    vertexVecFlat.push_back(p.Y());
    vertexVecFlat.push_back(0.f);

    uvVecFlat.push_back((p.X() + 1.f) / 2.f);
    uvVecFlat.push_back((p.Y() + 1.f) / 2.f);
  }
  loadVertex(vertexVecFlat.data(), vertexVecFlat.size(), uvVecFlat.data(), uvVecFlat.size(), id);
}

void Graphics::loadVertex(GLvoid *vvp, unsigned int vvSize, GLvoid *uvp, unsigned int uvSize, IDType id)
{
  if(_vboData._vboMap.count(id) > 0)
  {
    Log::Print("VBO " + ToString(id) + " already exists with ids=" + ToString(_vboData._vboMap[id]._t) + "," + ToString(_vboData._vboMap[id]._v));
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

void Graphics::LoadTexture(std::string texName)
{
  if(_textureMap.count(texName) == 0)
    _textureMap[texName] = loadTexture(texName);
  else
    Log::Print("Texture " + texName + " already exists with id=" + ToString(_textureMap[texName]));
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
  glVertexPointer(3, GL_FLOAT, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, tBuf);
  glTexCoordPointer(2, GL_FLOAT, 0, 0);

  glDrawArrays(GL_TRIANGLE_FAN, 0, vCount);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
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

  unsigned int vCount = obj.GetVArray().size();

  if(_textureMap.count(obj.GetTexture()) < 1)
    LoadTexture(obj.GetTexture());

  IDType vboID = obj.GetID();

  if(vCount > 0)
  {
    if(_vboData._vboMap.count(obj.GetID()) < 1)
      LoadVertex(obj.GetID(), obj.GetVArray()); // object looks like not pre-initialized, so let's load vertex data right now.
  }
  else
  {
    if (!_vboData._initWithDefault)
    {// this object will be the first to use 'default' VBO (rectangle -1..1), we'll remember its ID as a reference for all future objects that have no their own vertex data
      _vboData._defaultVBOID = obj.GetID();
      loadVertex(vertices, 4*3, uvs, 4*2, _vboData._defaultVBOID);
      _vboData._initWithDefault = true;
    }
    else
      vboID = _vboData._defaultVBOID; // object has no its own vertex data, so let's use default VBO reference that was initialized by the same 'rectangle'-based object somewhen before

    vCount = 4;
  }

  GLuint tex = _textureMap[obj.GetTexture()];
  VBO vbo = _vboData._vboMap[vboID];
  float opacity = 1.f - obj.GetDestroyProgress();

  draw(tex, vbo._v, vbo._t, vCount, posScreen, sizScreen, opacity);
}

void Graphics::frame()
{
  glClear(GL_COLOR_BUFFER_BIT);

  {
    drawObject(_game->GetBackgroundObject());

    if(!_game->IsGameOver())
    {
      drawObject(_game->GetHeroObject());

      drawObjects(_game->GetAsteroids());
      drawObjects(_game->GetBullets());

      drawObject(_game->GetPanelObject());
    }
    else
    {
      drawObject(_game->GetBlackObject());
      drawObject(_game->GetGameOverObject());
    }
  }
}

bool Graphics::Frame()
{
  clock_t frameStartTime = clock();
  if(dt(frameStartTime, _timeLastRedraw) >= DELAY)
  {
    _timeLastRedraw = frameStartTime;

    frame();
    return true;
  }
  return false;
}

void Graphics::delVBO(Graphics::VBO &vbo)
{
  glDeleteBuffers(1, &vbo._t);
  glDeleteBuffers(1, &vbo._v);
}

void Graphics::cleanup()
{  
  for (auto vbo : _vboData._vboMap)
    delVBO(vbo.second);

  for (auto tex : _textureMap)
    glDeleteTextures(1, &tex.second);

  _vboData._vboMap.clear();
  _textureMap     .clear();
  _vboData._initWithDefault = false;
}

Graphics::~Graphics()
{
  {
    glGetError();
    if (glGetError() > 0) // after double call it's still an error, context is likely dead
     return;
  }
  cleanup();
}
