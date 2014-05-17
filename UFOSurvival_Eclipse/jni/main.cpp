#include <jni.h>

#include <android/sensor.h>
#include <android_native_app_glue.h>

#include "game.h"
#include "graphics.h"

#include "log.h"

#define SHOOT_BORDER 0.1f

Game     *g_game = 0;
Graphics *g_graphics = 0;

bool g_restart = true;

AAssetManager *g_assetManager = 0;

struct engine
{
  struct android_app* app;

  ASensorManager* sensorManager;
  ASensorEventQueue* sensorEventQueue;

  EGLDisplay display;
  EGLSurface surface;
  EGLContext context;
  int32_t width;
  int32_t height;
};

static int engine_init_display(struct engine* engine)
{
  const EGLint attribs[] =
  {
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_NONE
  };
  EGLint w, h, dummy, format;
  EGLint numConfigs;
  EGLConfig config;
  EGLSurface surface;
  EGLContext context;

  EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

  eglInitialize(display, 0, 0);
  eglChooseConfig(display, attribs, &config, 1, &numConfigs);
  eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
  ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

  surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
  context = eglCreateContext(display, config, NULL, NULL);

  if(eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
  {
    Log::Print("Unable to eglMakeCurrent");
    return -1;
  }

  eglQuerySurface(display, surface, EGL_WIDTH,  &w);
  eglQuerySurface(display, surface, EGL_HEIGHT, &h);

  engine->display = display;
  engine->context = context;
  engine->surface = surface;
  engine->width  = w;
  engine->height = h;

  glShadeModel(GL_SMOOTH);
  glViewport(0, 0, engine->width, engine->height);

  return 0;
}

void restartGame(float w, float h)
{
  bool newHero = false;
  if(g_graphics)
  {
    delete g_graphics;
    newHero = true; // game was init previously
  }

  g_game->Restart(newHero);
  g_graphics = new Graphics();
  g_game->SetRatio((GLfloat)w / h);
  g_game->InitObjects();
  g_graphics->Init(g_game, Point(w, h));

  g_restart = false;
}

static void engine_draw_frame(struct engine* engine)
{
  if(engine->display == NULL)
    return;

  if(!g_assetManager)
    g_assetManager = engine->app->activity->assetManager;

  if(g_restart)
    restartGame(engine->width, engine->height);
  else
    g_game->Update();

  if(g_graphics->Frame())
    eglSwapBuffers(engine->display, engine->surface);
}

static void engine_term_display(struct engine* engine)
{
  if(engine->display != EGL_NO_DISPLAY)
  {
    eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if(engine->context != EGL_NO_CONTEXT)
      eglDestroyContext(engine->display, engine->context);

    if(engine->surface != EGL_NO_SURFACE)
      eglDestroySurface(engine->display, engine->surface);

    eglTerminate(engine->display);
  }
  engine->display = EGL_NO_DISPLAY;
  engine->context = EGL_NO_CONTEXT;
  engine->surface = EGL_NO_SURFACE;
}

static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
{
  struct engine* engine = (struct engine*)app->userData;
  if(AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {

    if(AInputEvent_getSource(event) == AINPUT_SOURCE_TOUCHSCREEN)
    {
      if(AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_DOWN)
      {
        if(g_game->IsGameOver())
          g_restart = true;

        else
        {
          float whereX =       AMotionEvent_getX(event, 0) / engine->width;
          float whereY = 1.f - AMotionEvent_getY(event, 0) / engine->height;

          Point at(whereX, whereY);

          if(whereX >       SHOOT_BORDER &&
             whereX < 1.f - SHOOT_BORDER &&
             whereY < 1.f - SHOOT_BORDER &&
             whereY >       SHOOT_BORDER)

            g_game->EngineFly(at);
          else
            g_game->Shoot(at);
        }
      }
      else if(AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_UP)
        g_game->EngineStop();
    }
    return 1;
  }
  return 0;
}

static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
  struct engine* engine = (struct engine*)app->userData;
  switch(cmd)
  {
  case APP_CMD_INIT_WINDOW:
    if(engine->app->window != NULL)
    {
      engine_init_display(engine);
      engine_draw_frame(engine);
    }
    break;
  case APP_CMD_TERM_WINDOW:
    engine_term_display(engine);
    break;
  }
}

void android_main(struct android_app* state)
{
  struct engine engine;
  app_dummy();

  memset(&engine, 0, sizeof(engine));
  state->userData = &engine;
  state->onAppCmd = engine_handle_cmd;
  state->onInputEvent = engine_handle_input;
  engine.app = state;

  engine.sensorManager = ASensorManager_getInstance();
  engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
    state->looper, LOOPER_ID_USER, NULL, NULL);

  g_game = new Game();

  while(1)
  {
    int ident;
    int events;
    struct android_poll_source* source;

    while((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
    {
      if(source != NULL)
        source->process(state, source);

      if(state->destroyRequested != 0)
      {
        engine_term_display(&engine);
        return;
      }
    }
    engine_draw_frame(&engine);
  }
}

