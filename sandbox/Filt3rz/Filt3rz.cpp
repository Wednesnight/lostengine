#include "Filt3rz.h"
#include "lost/event/Receive.h"
#include "lost/gl/StateParam.h"

using namespace std;
using namespace boost;
using namespace lost::common;
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
  contextOk = false;
  renderState = State::create(ClearColor::create(redColor));
}

Filt3rz::~Filt3rz()
{
}

void Filt3rz::setupFBOs()
{
    framebuffer.reset(new FrameBuffer());
    framebuffer->enable();
    tex.reset(new Texture(Vec2(256, 256)));
    framebuffer->attachColor(0, tex);
    DOUT("FBO complete: "<<framebuffer->isComplete());
    framebuffer->disable();
}

void Filt3rz::update(boost::shared_ptr<lost::application::Application> app)
{
  if(!contextOk)
  {
    window->context->makeCurrent();
    contextOk = true;
  }
  window->canvas->camera->apply();
  window->canvas->context->pushState(renderState);
  window->canvas->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
