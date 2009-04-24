#include "MeshTest.h"
#include "lost/event/Receive.h"
#include "lost/common/Logger.h"
#include "lost/application/ApplicationEvent.h"

#include "lost/rg/Draw.h"
#include "lost/rg/DepthTest.h"
#include "lost/rg/Camera.h"

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
    
    scene = rg::Node::create();
    scene->add(rg::Camera::create(camera2D));
    scene->add(rg::DepthTest::create(false));
    scene->add(rg::Draw::create(quad));
    scene->add(rg::Draw::create(quad2));
    scene->add(rg::Draw::create(quad3));
    scene->add(rg::Draw::create(line));
    scene->add(rg::Camera::create(camera3D));
    scene->add(rg::DepthTest::create(true));
    scene->add(rg::Draw::create(cube));
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
  
  scene->process(window->context);

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
