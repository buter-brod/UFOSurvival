#include <windows.h>  
#include "Graphics.h"
#include "Game.h"

#define SCREEN_W 1024.f
#define SCREEN_H 768.f

Game g_game;
Graphics g_graphics;

bool g_flying = false;

void display()
{
  g_graphics.Frame();
}

void idle()
{
  g_game.Update();
  glutPostRedisplay();
}

void mouseClick(int button, int state, int x, int y)
{
  if(button == GLUT_RIGHT_BUTTON)
  {
    if(state == GLUT_DOWN)
    {
      Point pos(float(x) / SCREEN_W, 1.f - float(y) / SCREEN_H);
      g_game.EngineFly(pos);
      g_flying = true;
    }
    else if(state == GLUT_UP)
    {
      g_game.StopEngine();
      g_flying = false;
    }
  }
  else
  {
  }
}

void mouse(int x, int y)
{
  if(g_flying)
  {
    Point pos(float(x) / SCREEN_W, 1.f - float(y) / SCREEN_H);
    g_game.EngineFly(pos);
  }
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitWindowSize(int(SCREEN_W), int(SCREEN_H));
  glutInitWindowPosition(200, 200);
  glutCreateWindow("Cepkolap");
    
  g_game.SetRatio(SCREEN_W / SCREEN_H);
  g_game.InitObjects();
  g_graphics.Init(&g_game, Point(SCREEN_W, SCREEN_H));
  
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutMouseFunc(mouseClick);
  glutMotionFunc(mouse);

  glutMainLoop();


	return 0;
}