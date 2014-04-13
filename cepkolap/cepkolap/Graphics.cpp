#include "Graphics.h"
#include <FreeImage.h>
#include "Utils.h"
#include "Log.h"

static float DELAY = 1.f / FPS;

static GLfloat vertices[][3] =
{
  {-1.f, -1.f, 0},
  { 1.f, -1.f, 0},
  { 1.f,  1.f, 0},
  {-1.f,  1.f, 0}
};

static GLfloat uvs[][2] =
{
  {0.f, 0.f},
  {1.f, 0.f},
  {1.f, 1.f},
  {0.f, 1.f}
};

GLubyte indices[] =
{
  0, 3, 1, 3, 2, 1
};

void Graphics::initGL()
{
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
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, /*GL_REPLACE*/ GL_MODULATE);

  glFrontFace(GL_CW);
  glShadeModel(GL_SMOOTH);
}
void Graphics::loadTextures()
{
  for(auto obj : _game->Objects())
    LoadTexture(obj.second.GetTexture());
}

void Graphics::Init(Game *game, Point size)
{
  _game = game;
  _size = size;

  initGL();
  loadTextures();
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

void Graphics::draw(GLuint tInd, Point pos, Point size)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
   
  glTranslatef( pos.X(),  pos.Y(), -1.0f);
  glScalef    (size.X(), size.Y(),  1.f);

  glVertexPointer  (3, GL_FLOAT, 0, vertices);
  glTexCoordPointer(2, GL_FLOAT, 0, uvs);

  glBindTexture(GL_TEXTURE_2D, tInd);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
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

void Graphics::drawObject(GameObject& obj)
{
  Point posScreen =  posToScreen(obj.GetPosition());
  Point sizScreen = sizeToScreen(obj.GetSize());

  draw(_textureMap[obj.GetTexture()], posScreen, sizScreen);

  {
    float x = obj.GetPosition().X();
    float y = obj.GetPosition().Y();
    
    float xs2 = obj.GetSize().X() / 2.f;
    float ys2 = obj.GetSize().Y() / 2.f;

    GLuint &tex = _textureMap[obj.GetTexture()];

    if(x > 1.f - xs2)
      draw(tex, posToScreen(Point(x - 1.f, y      )), sizScreen);

    if(x < 0.f + xs2)
      draw(tex, posToScreen(Point(x + 1.f, y      )), sizScreen);

    if(y > 1.f - ys2)
      draw(tex, posToScreen(Point(x,       y - 1.f)), sizScreen);

    if(y < 0.f + ys2)
      draw(tex, posToScreen(Point(x,       y + 1.f)), sizScreen);
  }
}

void Graphics::frame()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  for(auto obj : _game->Objects())
    drawObject(obj.second);

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
