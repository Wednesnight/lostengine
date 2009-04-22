#include "lost/gl/gl.h"
#include "lost/gl/Context.h"
#include "lost/application/Application.h"
#include "lost/common/Logger.h"
#include "lost/common/FpsMeter.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/platform/Platform.h"
#include "lost/application/KeyEvent.h"
#include "lost/event/Receive.h"
#include "lost/gl/State.h"
#include "lost/camera/Camera.h"
#include "lost/bitmap/Bitmap.h"

using namespace lost;
using namespace boost;
using namespace lost::application;
using namespace lost::gl;
using namespace lost::math;
using namespace lost::common;
using namespace lost::bitmap;

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
  lost::shared_ptr<Application> app;
  lost::shared_ptr<Window> mainWindow;
  gl::SharedState renderState;
  lost::shared_ptr<lost::common::FpsMeter> fpsMeter;

  double ds;

  void drawScene(const lost::shared_ptr<gl::Canvas>& canvas, double deltaSec)
  {
    canvas->context->makeCurrent();
    canvas->context->pushState(renderState);

    canvas->camera->apply();
    canvas->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    bmp->clear(blackColor);
    ds+=deltaSec*10;
    bmp->hline(50+10*cos(ds), 0,100,redColor);
    tex->subImage(Vec2(0,0), bmp);

    canvas->setColor(common::whiteColor);
    canvas->drawRectTextured(Rect(0,0,windowWidth,windowHeight), tex, false);
    fpsMeter->render(windowWidth-200, 0, canvas, deltaSec);

    canvas->context->popState();
    canvas->context->swapBuffers();
  }

  void render(const double deltaSec)
  {
    drawScene(mainWindow->canvas, deltaSec);
  }

  void mainLoop()
  {
    static MeteredFunction meteredRender(boost::bind(&MyAppController::render, this, _1));
    meteredRender();
    app->processEvents();
  }

  void keyHandler(lost::shared_ptr<KeyEvent> event)
  {
    if (event->key == K_ESCAPE) app->quit();
  }

public:
  uint32_t windowWidth;
  uint32_t windowHeight;

  shared_ptr<Bitmap> bmp;
  shared_ptr<Texture> tex;

  MyAppController()
  {
    app = Application::create(boost::bind(&MyAppController::mainLoop, this));

    windowWidth = 512;
    windowHeight = 512;
    ds = 0;
    mainWindow = app->createWindow("window", WindowParams("Application", lost::math::Rect(100, 100, windowWidth, windowHeight)));
    bmp.reset(new Bitmap(windowWidth, windowHeight, Bitmap::COMPONENTS_RGB));
    bmp->clear(Color(0,0,0));
    bmp->hline(50, 0, 100, redColor);
    bmp->vline(50, 0, 100, greenColor);
    tex.reset(new Texture(bmp));
    fpsMeter.reset(new lost::common::FpsMeter());

    renderState = gl::State::create(gl::ClearColor::create(lost::common::blackColor));
    app->addEventListener(lost::application::KeyEvent::KEY_DOWN(), lost::event::receive<KeyEvent>(boost::bind(&MyAppController::keyHandler, this, _1)));
  }

  int run()
  {
    app->run();
    return 0;
  }
};

int main (int argc, const char * argv[])
{
  MyAppController controller;
  return controller.run();
}
