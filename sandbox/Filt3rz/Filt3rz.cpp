#include "Filt3rz.h"
#include "lost/event/Receive.h"
#include "lost/gl/StateParam.h"

using namespace std;
using namespace boost;
using namespace lost::common;
using namespace lost::camera;
using namespace lost::application;
using namespace lost::event;
using namespace lost::math;
using namespace lost::gl;

Filt3rz::Filt3rz()
{
  app = Application::create(bind(&Filt3rz::update, this, _1));
  app->runLoopWaitsForEvents(true);
  app->addEventListener(lost::application::KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Filt3rz::keyHandler, this, _1)));
    
  window = app->createWindow("window", WindowParams("Filt3rz", Rect(0,0,800,600)));
  window->context->makeCurrent();
  setupFBOs();  
  renderState = State::create(ClearColor::create(redColor));
  fboRenderState = State::create(ClearColor::create(whiteColor));
}

Filt3rz::~Filt3rz()
{
  DOUT("shutting down");
}

void Filt3rz::setupFBOs()
{
  fboSize.width = 256;
  fboSize.height = 256;

  fboViewport.x = 0;
  fboViewport.y = 0;
  fboViewport.width = fboSize.width;
  fboViewport.height = fboSize.height;

  framebuffer.reset(new FrameBuffer());
  framebuffer->enable();
  tex.reset(new Texture(fboSize));
  framebuffer->attachColor(0, tex);
  tex->filter(GL_NEAREST);
  DOUT("FBO complete: "<<framebuffer->isComplete());
  framebuffer->disable();
  
  fboCam.reset(new Camera2D(window->context, fboViewport));
  fboCanvas.reset(new Canvas(window->context, fboCam));
}

void Filt3rz::renderFbo()
{
  framebuffer->enable();
  fboCanvas->camera->apply();
  fboCanvas->context->pushState(fboRenderState);
  fboCanvas->clear(GL_COLOR_BUFFER_BIT);
  fboCanvas->setColor(blackColor);
  glMatrixMode(GL_MODELVIEW);GLDEBUG;
  glLoadIdentity();GLDEBUG;
  fboCanvas->drawLine(Vec2(0,0), Vec2(100,100));
  fboCanvas->drawPoint(Vec2(128,128));
  fboCanvas->context->popState();
  framebuffer->disable();
}

void Filt3rz::update(boost::shared_ptr<lost::application::Application> app)
{
  // don't call this everywhere! call it at the beginning of the draw method.
  // makeCurrent won't perform unnecessary context switches
  window->context->makeCurrent(); 
  
  renderFbo();

  window->canvas->camera->apply();
  window->canvas->context->pushState(renderState);
  window->canvas->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  window->canvas->setColor(whiteColor);
  glMatrixMode(GL_MODELVIEW);GLDEBUG;
  glLoadIdentity();GLDEBUG;
  window->canvas->drawRectTextured(Rect(400,300,fboSize.width, fboSize.height), tex, false);  
  fboCanvas->drawLine(Vec2(0,0), Vec2(100,100));
  fboCanvas->drawPoint(Vec2(128,128));
  window->canvas->context->popState();
  window->context->swapBuffers();
}

void Filt3rz::keyHandler(boost::shared_ptr<lost::application::KeyEvent> event)
{
    if (event->key == K_ESCAPE) app->quit();
}

void Filt3rz::run()
{
  app->run();
}
