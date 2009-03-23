#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/event/Receive.h"
#include "lost/application/KeyEvent.h"
#include "lost/gl/FrameBuffer.h"

using namespace std;
using namespace boost;
using namespace lost::common;
using namespace lost::application;
using namespace lost::event;
using namespace lost::math;
using namespace lost::gl;

boost::shared_ptr<Application> app;
shared_ptr<Window> window;
shared_ptr<FrameBuffer> framebuffer;
shared_ptr<Texture> tex;

void keyHandler(boost::shared_ptr<KeyEvent> event)
{
    if (event->key == K_ESCAPE) app->quit();
}

void run(shared_ptr<Application> app)
{
}

void setupFBOs()
{
    framebuffer.reset(new FrameBuffer());
    tex.reset(new Texture(Vec2(256, 256)));
    framebuffer->attach(0, tex);
    DOUT("FBO complete: "<<framebuffer->isComplete());
}

int main(int argn, char** args)
{
  try
  {
    app = Application::create(run);
    window = app->createWindow("window", WindowParams("Filt3rz", Rect(0,0,800,600)));
//    app->runLoopWaitsForEvents(true);
    app->addEventListener(lost::application::KeyEvent::KEY_DOWN(), receive<KeyEvent>(keyHandler));
    setupFBOs();
    app->run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}

