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
using namespace lost::meter;
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

  meter.reset(new Meter());
  lua->globals["meter"] = MeshPtr(meter->mesh);

  bool result = Tasklet::startup();
  if (result)
  {    
    scene = lua->globals["scene"]; // required for drawing
    rg::DrawPtr cubeDrawNode = static_pointer_cast<rg::Draw>(scene->recursiveFindByName("cube"));
    if(cubeDrawNode) // required for updates
    {
      cube = cubeDrawNode->mesh;
    }
    else
    {
      EOUT("CUBE NOT FOUND !!!!");
    }
    
  }
  
  return result;
}

void MeshTest::update(double dt)
{
  angle = fmod(dt*50+angle, 360);
  if(cube)
  {
    cube->modelTransform = MatrixRotX(angle) * MatrixRotY(angle);
  }
}

void MeshTest::draw()
{
  scene->process(window->context);
  window->context->swapBuffers();  
}

bool MeshTest::main()
{
  double currentSec = lost::platform::currentTimeSeconds();
  double delta = currentSec - passedSec;
  update(delta);
  draw();
    
  meter->update(1.0f/delta);
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
