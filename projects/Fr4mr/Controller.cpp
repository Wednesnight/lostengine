#include "Controller.h"

#include "lost/event/Receive.h"
#include "lost/camera/Camera.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/common/Logger.h"
#include "lost/gl/Context.h"
#include "lost/gl/gl.h"
#include <math.h>

using namespace lost::application;
using namespace lost;
using namespace lost::gl;
using namespace lost::common;
using namespace lost::math;
using namespace lost::font;
using namespace boost;
using namespace lost::camera;

struct MeteredFunction
{
private:
  boost::function<void (const double deltaSec)> function;
  double passedSec;
public:
  MeteredFunction(const boost::function<void (const double deltaSec)> inFunction)
  : function(inFunction),
    passedSec(lost::platform::currentTimeSeconds())
  {
  }
  
  inline void operator()()
  {
    double currentSec = lost::platform::currentTimeSeconds();
    function(currentSec - passedSec);
    passedSec = currentSec;
  }
};

void MyAppController::shaderInit()
{
  program = loadShader(app->loader, "lattice");
  program->enable();
  program->validate();
  if(!program->validated())
  {
    DOUT("Problem found during validation: \n"<<program->log())
  }
  else
  {
    DOUT("Program validated OK");
  }
  (*program)["LightPosition"] = Vec3(-1,.7,10);
  (*program)["LightColor"]    = Color(1,1,1);
  (*program)["EyePosition"]   = fullscreen3dCam->position();
  (*program)["Specular"]      = Color(1,1,.1);
  (*program)["Ambient"]       = Color(.1,.5,1);
  (*program)["Kd"]            = 0.8f;
  (*program)["Scale"]         = Vec2(0.7, 3.7);
  (*program)["Threshold"]     = Vec2(.3, .2);
  (*program)["SurfaceColor"]  = Color(1,.1,.1);
  program->disable();
}

MyAppController::MyAppController()
{
  app = Application::create(boost::bind(&MyAppController::mainLoop, this));
  uint32_t screenWidth = 800;
  uint32_t screenHeight = 600;
  mainWindow = app->createWindow("window", WindowParams("Application", lost::math::Rect(100, 100, screenWidth, screenHeight)));
  mainWindow->canvas->context->makeCurrent(); // just in case, for all following operations
  renderState = gl::State::create(gl::ClearColor::create(lost::common::blackColor));
  fpsMeter.reset(new lost::common::FpsMeter());
  app->addEventListener(lost::application::KeyEvent::KEY_DOWN(), lost::event::receive<KeyEvent>(boost::bind(&MyAppController::keyHandler, this, _1)));

  fullscreen3dCam.reset(new Camera3D(mainWindow->canvas->context, Rect(0,0,screenWidth, screenHeight)));
  fullscreen3dCanvas.reset(new Canvas(mainWindow->context, fullscreen3dCam));
  fullscreen3dCam->position(Vec3(2,3,5));
  fullscreen3dCam->target(Vec3(1,1,0));
  fullscreen3dCam->stickToTarget(true);
  fullscreen3dCam->fovY(90);
  shaderInit(); // load shaders here cos they need cam pos

  modelRenderState = State::create(ClearColor::create(blackColor), Blend::create(false), DepthTest::create(true));
  angle = 0;
  
  lib.reset(new freetype::Library());
  fnt.reset(new TrueTypeFont(lib, app->loader->load("miserable.ttf")));
  logo = fnt->render("WEDNESNIGHT", 100);
  logo->renderState = State::create(Texture2D::create(true), Blend::create(true), BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                  DepthTest::create(true), NormalArray::create(false), VertexArray::create(true), TextureArray::create(true));
  fnt->atlas->filter(GL_LINEAR);

}

void MyAppController::drawModel(const boost::shared_ptr<lost::gl::Canvas>& canvas, double deltaSec)
{
  canvas->context->makeCurrent();
  canvas->context->pushState(modelRenderState);

  canvas->camera->apply();
  canvas->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  canvas->setColor(common::redColor);
  angle = fmod(deltaSec*100+angle, 360);
  float cubeSize = 1;
  glTranslatef(-1, .2,0);
  glRotatef(angle, 0,1, 0);
  program->enable();
  canvas->drawSolidCube(cubeSize);
  glTranslatef(0,cubeSize,0);
  canvas->drawSolidCube(cubeSize);
  glTranslatef(0,cubeSize,0);
  canvas->drawSolidCube(cubeSize);
  glTranslatef(cubeSize,-cubeSize,0);
  canvas->drawSolidCube(cubeSize);
  program->disable();
  canvas->context->popState();

  glLoadIdentity();
  math::Vec3 target = fullscreen3dCam->target();
  glTranslatef(target.x, target.y, target.z);
  canvas->drawAxes(math::Vec3(1,1,1));

  glLoadIdentity();
  canvas->setColor(whiteColor);
  glTranslatef(1,0,0);
  float scale = .01; //.01;
  glScalef(scale,scale,scale);
  logo->render(canvas);

  canvas->context->popState();
  canvas->context->swapBuffers();
}


void MyAppController::drawScene(const boost::shared_ptr<gl::Canvas>& canvas, double deltaSec)
{
  canvas->context->makeCurrent();
  canvas->context->pushState(renderState);

  canvas->camera->apply();
  canvas->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  math::Vec2 p1(415, 215);
  math::Vec2 p2(415, 240);
  math::Vec2 p3(520, 550);
  math::Vec2 p4(620, 320);

  canvas->setColor(common::redColor);
  canvas->drawLine(p1, p2);
  canvas->drawLine(p3, p4);

  canvas->setColor(common::whiteColor);
  canvas->drawBezierCurve(p1, p2, p3, p4, 100);

  canvas->setColor(common::greenColor);
  canvas->drawRectOutlineRounded(math::Rect(100, 100, 200, 200), math::Vec2(50, 75));

  canvas->setColor(common::whiteColor);
  canvas->drawRectOutline(math::Rect(100, 500, 50, 50));
  canvas->setColor(common::redColor);
  canvas->drawRectFilled(math::Rect(100, 500, 50, 50));

  canvas->setColor(common::whiteColor);
  canvas->drawRectFilled(math::Rect(125, 125, 150, 150));
  canvas->setColor(common::redColor);
  canvas->drawRectFilledRounded(math::Rect(125, 125, 150, 150), math::Vec2(25, 50));

  canvas->setColor(common::redColor);
  canvas->drawBezierCurveFilled(math::Vec2(10, 10), math::Vec2(10, 100), math::Vec2(10, 100), math::Vec2(100, 100), math::Vec2(100.5, 9.5));
  canvas->setColor(common::whiteColor);
  canvas->drawBezierCurve(math::Vec2(10, 10), math::Vec2(10, 100), math::Vec2(10, 100), math::Vec2(100, 100));

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  fpsMeter->render(0, 0, canvas, deltaSec);

  canvas->context->popState();
  canvas->context->swapBuffers();
}

void MyAppController::render(double deltaSec)
{
//  drawScene(mainWindow->canvas, deltaSec);
  drawModel(fullscreen3dCanvas, deltaSec);
}

void MyAppController::mainLoop()
{
  static MeteredFunction meteredRender(boost::bind(&MyAppController::render, this, _1));
  meteredRender();
  app->processEvents();
}

void MyAppController::keyHandler(boost::shared_ptr<KeyEvent> event)
{
  if (event->key == K_ESCAPE) app->quit();
  float stepsize = 2.0;
  switch(event->key)
  {
    case K_ESCAPE:app->quit();break;
    case K_A:fullscreen3dCam->rotate(Vec3(0,stepsize,0));break;
    case K_D:fullscreen3dCam->rotate(Vec3(0,-stepsize,0));break;
    case K_W:fullscreen3dCam->rotate(Vec3(stepsize,0,0));break;
    case K_S:fullscreen3dCam->rotate(Vec3(-stepsize,0,0));break;
    case K_Q:fullscreen3dCam->move(Vec3(0,0,-stepsize/8));break;
    case K_E:fullscreen3dCam->move(Vec3(0,0,stepsize/8));break;
  }
}


int MyAppController::run()
{
  app->run();
  return 0;
}
