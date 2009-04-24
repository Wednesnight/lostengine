#include "MeshTest.h"
#include "lost/event/Receive.h"
#include "lost/common/Logger.h"
#include "lost/application/ApplicationEvent.h"

using namespace std;
using namespace lost;
using namespace lost::gl;
using namespace lost::event;
using namespace lost::common;
using namespace lost::math;
using namespace lost::event;
using namespace lost::application;
using namespace lost::camera;
using namespace lost::mesh;
using namespace luabind;

MeshTest::MeshTest()
: UiTasklet(WindowParams("MeshTest", Rect(50,200,640,480)))
{
  passedSec = lost::platform::currentTimeSeconds();
  angle = 0;
}


bool MeshTest::startup()
{
  // key handlers
  eventDispatcher->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&MeshTest::keyHandler, this, _1)));
  // context
  window->context->clearColor(blackColor);

  bool result = Tasklet::startup();
  if (result)
  {
    camera2D = (Camera2DPtr)lua->globals["camera2D"];
    camera3D = lua->globals["camera3D"];
    quad = lua->globals["quad"];
    quad2 = lua->globals["quad2"];
    quad3 = lua->globals["quad3"];
    line = lua->globals["line"];
    cube = lua->globals["cube"];
  }
  
  return result;
}

void MeshTest::update(double dt)
{
  angle = fmod(dt*50+angle, 360);
  cube->modelTransform = MatrixRotX(angle) * MatrixRotY(angle);
}

void MeshTest::draw()
{
  window->context->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // draw 2D over 3D scene
  window->context->camera(camera2D);
  window->context->depthTest(false);
  window->context->draw(quad);
  window->context->draw(quad2);
  window->context->draw(quad3);
  window->context->draw(line);

  // draw 3D
  window->context->camera(camera3D);
  window->context->depthTest(true);
  window->context->draw(cube);

/*ctx->color(redColor);
glBegin(GL_POINTS);
  glVertex2f(0, 0);
  glVertex2f(639, 479);
glEnd();*/

  window->context->swapBuffers();  
}

bool MeshTest::main()
{
  double currentSec = lost::platform::currentTimeSeconds();
  double delta = currentSec - passedSec;
  update(delta);
  draw();
  
  passedSec = currentSec; 
  return true;
}

bool MeshTest::shutdown()
{
  return true;
}

void MeshTest::keyHandler(KeyEventPtr event)
{
  DOUT("");
  if (event->key == K_ESCAPE) 
    eventDispatcher->dispatchEvent(ApplicationEventPtr(new ApplicationEvent(ApplicationEvent::QUIT())));
}
