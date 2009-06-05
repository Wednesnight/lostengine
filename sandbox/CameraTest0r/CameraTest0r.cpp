#include "CameraTest0r.h"
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
using namespace lost::meter;
using namespace luabind;

CameraTest0r::CameraTest0r()
: UiTasklet(WindowParams("CameraTest0r", Rect(50,200,640,480)))
{
  passedSec = lost::platform::currentTimeSeconds();
}


bool CameraTest0r::startup()
{
  // key handlers
  eventDispatcher->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&CameraTest0r::keyHandler, this, _1)));

  bool result = Tasklet::startup();
  if (result)
  {
    scene = lua->globals["scene"]; // required for drawing
  }
  return result;
}

void CameraTest0r::update(double dt)
{
  luabind::call_function<void>(lua->globals["update"], dt);
}

void CameraTest0r::draw()
{
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  scene->process(window->context);
  window->context->swapBuffers();  
}

bool CameraTest0r::main()
{
  double currentSec = lost::platform::currentTimeSeconds();
  double delta = currentSec - passedSec;
  update(delta);
  draw();  
  passedSec = currentSec; 
  return true;
}

void CameraTest0r::keyHandler(KeyEventPtr event)
{
  if (event->key == K_ESCAPE) 
    eventDispatcher->dispatchEvent(ApplicationEventPtr(new ApplicationEvent(ApplicationEvent::QUIT())));
}
