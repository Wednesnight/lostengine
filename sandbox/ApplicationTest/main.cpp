#include "lost/gl/gl.h"
#include "lost/gl/Context.h"
#include "lost/application/Application.h"
#include "lost/common/Logger.h"
#include "lost/common/FpsMeter.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/platform/Platform.h"
#include "lost/application/KeyEvent.h"
#include "lost/event/Receive.h"
#include "lost/application/gl/State.h"
#include "lost/camera/Camera.h"

using namespace lost::application;

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

struct MyAppController
{
private:
  boost::shared_ptr<Application> app;
  boost::shared_ptr<Window> mainWindow;
  boost::shared_ptr<Window> secondWindow;
  gl::SharedState renderState;
  boost::shared_ptr<lost::common::FpsMeter> fpsMeter;

  void drawScene(const boost::shared_ptr<gl::Canvas>& canvas)
  {
    canvas->context->makeCurrent();
    canvas->context->pushState(renderState);

    canvas->camera->apply();
    canvas->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    canvas->drawLine(lost::math::Vec2(0,0), lost::math::Vec2(canvas->camera->viewport.width, canvas->camera->viewport.height));

    canvas->context->popState();
    canvas->context->swapBuffers();
  }

  void drawFPSMeter(const boost::shared_ptr<gl::Canvas>& canvas, const double& deltaSec)
  {
    canvas->context->makeCurrent();
    canvas->context->pushState(renderState);

    canvas->camera->apply();
    canvas->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    fpsMeter->render(0, 0, canvas, deltaSec);

    canvas->context->popState();
    canvas->context->swapBuffers();
  }

  void render(const double deltaSec)
  {
    drawScene(mainWindow->canvas);
    drawFPSMeter(secondWindow->canvas, deltaSec);
  }

  void mainLoop()
  {
    static MeteredFunction meteredRender(boost::bind(&MyAppController::render, this, _1));
    meteredRender();
    app->processEvents();
  }

  void keyHandler(boost::shared_ptr<KeyEvent> event)
  {
    if (event->key == K_ESCAPE) app->quit();
  }

public:
  MyAppController()
  {
    app = Application::create(boost::bind(&MyAppController::mainLoop, this));

    mainWindow = app->createWindow("window", WindowParams("Application", lost::math::Rect(100, 100, 800, 600)));
    secondWindow = app->createWindow("window2", WindowParams("FPSMeter", lost::math::Rect(100, 100, 160, 100)));

    renderState = gl::State::create(gl::ClearColor::create(lost::common::blackColor));

    fpsMeter.reset(new lost::common::FpsMeter());

    app->addEventListener(lost::application::KeyEvent::KEY_DOWN(), lost::event::receive<KeyEvent>(boost::bind(&MyAppController::keyHandler, this, _1)));
  }

  int run()
  {
    app->run();
    return 0;
  }
};

int testingCPP()
{
  MyAppController controller;
  return controller.run();
}

int testingLUA()
{
  boost::shared_ptr<Application> app = Application::create(boost::filesystem::path("init.lua"));
  app->run();
  return 0;
}

int main (int argc, const char * argv[])
{
  return testingCPP();
//  return testingLUA();
}
