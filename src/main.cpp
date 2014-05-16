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
  glutSwapBuffers();
}

void idle()
{
  g_game.Update();
  glutPostRedisplay();
}

void mouseClick(int button, int state, int x, int y)
{
  Point pos(float(x) / SCREEN_W, 1.f - float(y) / SCREEN_H);

  if (g_game.IsGameOver())
  {
    g_game.Restart(true);
  }
  
  else if(button == GLUT_RIGHT_BUTTON)
  {
    if(state == GLUT_DOWN)
    {
      g_game.EngineFly(pos);
      g_flying = true;
    }
    else if(state == GLUT_UP)
    {
      g_game.EngineStop();
      g_flying = false;
    }
  }
  else if (button == GLUT_LEFT_BUTTON)
  {
    if(state == GLUT_DOWN)
    {
      g_game.Shoot(pos);
    }
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
  glutInitDisplayMode (GLUT_DOUBLE);
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