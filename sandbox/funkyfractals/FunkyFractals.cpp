#include "FunkyFractals.h"

#include <boost/bind.hpp>
#include "lost/math/Rect.h"
#include "lost/common/Color.h"
#include "lost/platform/Platform.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/event/Receive.h"

using namespace boost;
using namespace lost::application;
using namespace lost::camera;
using namespace lost::common;
using namespace lost::event;
using namespace lost::gl;
using namespace lost::lsystem;
using namespace lost::math;
using namespace lost::platform;


struct Line
{
  Vec3 start;
  Vec3 end;

  Line(const Vec3& inStart, const Vec3& inEnd)
  : start(inStart),
    end(inEnd)
  {
  }

  void draw(const CanvasPtr& canvas, const Vec3& offset)
  {
    canvas->setColor(whiteColor);
    canvas->drawSolidCube(1);
  }
};


FunkyFractals::FunkyFractals()
: application(Application::create(bind(&FunkyFractals::redraw, this, _1)))
{
  // create window
  camera.reset(new Camera3D(window->canvas->context, window->canvas->camera->viewport));
  camera->fovY(45.0f);
  camera->depth(Vec2(1.0f, 100.0f));
  camera->position(Vec3(1,2,20));
  camera->target(Vec3(0,0,0));
  camera->stickToTarget(true);  
  canvas.reset(new Canvas(window->context, camera));
  
  renderState = State::create(ClearColor::create(whiteColor), DepthTest::create(true));
  fpsMeter.reset(new FpsMeter());
  setupLightShader(window->canvas);

  application->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&FunkyFractals::keyHandler, this, _1)));

  lsystem.reset(new LSystem());
  lsystemVars['F'] = "FzXFZxFzXFzXF";
  lsystemState.reset(new LSystemState("FzFzFzF", lsystemVars, Vec3(5,0,90)));
  lsystem->advance(lsystemState, 6);
}

void FunkyFractals::setupLightShader(const CanvasPtr& canvas)
{
  lightShader = loadShader(application->loader, "lighting");
  lightShader->enable();
  lightShader->validate();
  if(!lightShader->validated())
  {
    EOUT("Problem found during validation: \n" << lightShader->log());
  }
  else
  {
    DOUT("Program validated OK");
  }
  (*lightShader)["LightPosition"] = Vec3(-1,.7,10);
  (*lightShader)["LightColor"]    = Color(1,1,1);
  (*lightShader)["EyePosition"]   = camera->position();
  (*lightShader)["Specular"]      = Color(1,1,.1);
  (*lightShader)["Ambient"]       = Color(.1,.5,1);
  (*lightShader)["Kd"]            = 0.8f;
  (*lightShader)["Scale"]         = Vec2(0.7, 3.7);
  (*lightShader)["Threshold"]     = Vec2(.3, .2);
  (*lightShader)["SurfaceColor"]  = Color(1,.1,.1);
  lightShader->disable();
}

void FunkyFractals::run()
{
  application->run();
}

void FunkyFractals::redraw(ApplicationPtr application)
{
  static double passedSec = currentTimeSeconds();
  double currentSec = currentTimeSeconds();
  double delta = currentSec - passedSec;

  window->context->makeCurrent();
  window->context->pushState(renderState);
  window->canvas->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  canvas->camera->apply();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  canvas->setColor(whiteColor);
  lightShader->enable();
  Matrix matrix;
  matrix.initTranslation(Vec3(0,0,0));
  lsystem->walk(lsystemState, matrix, bind(&FunkyFractals::renderLSystemNode, this, _1, _2));
  lightShader->disable();
  
  window->canvas->camera->apply();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  fpsMeter->render(window->canvas->camera->viewport.width - fpsMeter->width, 0, window->canvas, delta);
  
  window->context->popState();
  window->context->swapBuffers();

  passedSec = currentTimeSeconds();
}

void FunkyFractals::renderLSystemNode(Matrix& matrix, LSystemResult& result)
{
  if (result.drawable)
  {
    Line l(Vec3(0, 0, 0), Vec3(0, 2, 0));
    l.draw(canvas, Vec3(0, 0, 0));
    
    Matrix matrix;
    matrix.initTranslation(Vec3(0, 2, 0));
    result.matrix = result.matrix * matrix;
  }
  else
  {
    glLoadMatrixf(result.matrix.m); GLDEBUG;
  }
}

void FunkyFractals::keyHandler(KeyEventPtr event)
{
  float stepsize = 2.0;
  switch(event->key)
  {
    case K_ESCAPE:application->quit();break;
    case K_A:camera->rotate(Vec3(0,stepsize,0));break;
    case K_D:camera->rotate(Vec3(0,-stepsize,0));break;
    case K_W:camera->rotate(Vec3(stepsize,0,0));break;
    case K_S:camera->rotate(Vec3(-stepsize,0,0));break;
    case K_Q:camera->move(Vec3(0,0,-stepsize/8));break;
    case K_E:camera->move(Vec3(0,0,stepsize/8));break;
  }
}
