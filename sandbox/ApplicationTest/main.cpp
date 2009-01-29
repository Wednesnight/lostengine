#include "lost/gl/gl.h"
#include "lost/gl/Context.h"
#include "lost/application/Application.h"
#include "lost/common/Logger.h"
#include "lost/common/FpsMeter.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/platform/Platform.h"
#include "lost/application/KeySym.h"
#include "lost/application/KeyEvent.h"
#include "lost/event/Receive.h"

using namespace lost::application;

struct MyAppController
{
private:
  boost::shared_ptr<Application> app;
  boost::shared_ptr<Window> mainWindow;
  boost::shared_ptr<Window> secondWindow;
  boost::shared_ptr<lost::gl::Context> context;
  boost::shared_ptr<lost::gl::State> state;
  boost::shared_ptr<lost::common::FpsMeter> fpsMeter;
  boost::shared_ptr<lost::common::DisplayAttributes> displayAttributes;
  double passedSec;

  void drawWindow1()
  {
    mainWindow->context->makeCurrent();
    glViewport(0, 0, 800, 600);
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glBegin(GL_QUADS);
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glVertex3f(-0.5, -0.5, 0.0);
    
    glColor4f(0.0, 1.0, 0.0, 1.0);
    glVertex3f(0.5, -0.5, 0.0);
    
    glColor4f(0.0, 0.0, 1.0, 1.0);
    glVertex3f(0.5, 0.5, 0.0);
    
    glColor4f(1.0, 0.0, 1.0, 1.0);
    glVertex3f(-0.5, 0.5, 0.0);
    glEnd();
    
    glRotatef(-0.5, 0.6, -0.6, 0.5);
    mainWindow->context->swapBuffers();
  }

  void drawWindow2(const double& deltaSec)
  {
    secondWindow->context->makeCurrent();
    glViewport(0, 0, 160, 100);
    context->pushState(state);
    context->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    context->set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(160,100));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    fpsMeter->render(0, 0, deltaSec);
    context->popState();
    secondWindow->context->swapBuffers();
  }

  void mainLoop()
  {
    double currentSec = lost::platform::currentTimeSeconds();
    drawWindow1();
    drawWindow2(currentSec - passedSec);
    app->processEvents();
    passedSec = currentSec;
  }

  void keyHandler(boost::shared_ptr<KeyEvent> event)
  {
    if (event->key == 53) app->quit();
  }

public:
  MyAppController()
  : passedSec(lost::platform::currentTimeSeconds())
  {
    app = Application::create(boost::bind(&MyAppController::mainLoop, this));

    mainWindow = app->createWindow("window", WindowParams("Application", lost::math::Vec2(800, 600), lost::math::Vec2(100, 100)));

    secondWindow = app->createWindow("window2", WindowParams("FPSMeter", lost::math::Vec2(160, 100), lost::math::Vec2(100, 100)));
    displayAttributes.reset(new lost::common::DisplayAttributes);
    context.reset(new lost::gl::Context(displayAttributes));
    state = context->copyState();
    state->clearColor = lost::common::Color(0,0,0,0);
    fpsMeter.reset(new lost::common::FpsMeter(context));

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
//  return testingCPP();
  return testingLUA();
}
