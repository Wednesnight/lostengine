#include "MeshTest.h"
#include "lost/event/Receive.h"
#include "lost/common/Logger.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/model/Loader.h"

using namespace std;
using namespace lost;
using namespace lost::gl;
using namespace lost::event;
using namespace lost::common;
using namespace lost::math;
using namespace lost::event;
using namespace lost::application;

MeshTest::MeshTest()
: UiTasklet(WindowParams("MeshTest", Rect(50,200,640,480)))
{
  
}


bool MeshTest::startup()
{
  DOUT("");
  eventDispatcher->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&MeshTest::keyHandler, this, _1)));
  window->context->makeCurrent();
  renderstate = State::create(ClearColor::create(blackColor), DepthTest::create(false));
  linestate = State::create();
  line = new mesh::Line2D;
  cube = lost::model::Loader::obj(loader, "cube_tri.obj");
  
  return true;
}

void MeshTest::draw2D()
{
  window->canvas->camera->apply();
  window->canvas->context->pushState(renderstate);
  window->canvas->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  window->canvas->setColor(whiteColor);
  glMatrixMode(GL_MODELVIEW);GLDEBUG;
  glLoadIdentity();GLDEBUG;
  line->update(Vec2(0,0), Vec2(400,400));
  line->draw(window->context);
  window->canvas->context->popState();
}

void MeshTest::draw3D()
{
  cube->draw(window->context);
}

bool MeshTest::main()
{
  window->context->makeCurrent();
  
  try
  {
    draw2D();
    draw3D();
  }
  catch(std::exception& ex)
  {
    EOUT("caught error: "<<ex.what());
  }
  
  window->context->swapBuffers();   
  return true;
}

bool MeshTest::shutdown()
{
  DOUT("");
  return true;
}

void MeshTest::keyHandler(KeyEventPtr event)
{
  DOUT("");
  if (event->key == K_ESCAPE) 
    eventDispatcher->dispatchEvent(ApplicationEventPtr(new ApplicationEvent(ApplicationEvent::QUIT())));
}
