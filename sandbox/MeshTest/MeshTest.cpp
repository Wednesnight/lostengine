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
  
}


bool MeshTest::startup()
{
  // key handlers
  eventDispatcher->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&MeshTest::keyHandler, this, _1)));
  // context
  window->context->makeCurrent();
  ctx.reset(new XContext(window->context));
  ctx->makeCurrent();
  ctx->clearColor(blackColor);
  // cam 2D
  camera2D = window->canvas->camera;
  // cam 3D
  camera3D.reset(new Camera3D(window->context, Rect(0, 0, window->canvas->camera->viewport.width, window->canvas->camera->viewport.height)));
  camera3D->fovY(45.0f);
  camera3D->depth(Vec2(1.0f, 100.0f));
  camera3D->position(Vec3(1,2,2));
  camera3D->target(Vec3(0,0,0));
  camera3D->stickToTarget(true);  

  luabind::globals(*interpreter)["camera3D"] = camera3D;
  luabind::object func = luabind::globals(*interpreter)["init"];
  call_function<void>(func, loader);
  quad = object_cast<Quad2DPtr>(globals(*interpreter)["quad"]);
  line = object_cast<Line2DPtr>(globals(*interpreter)["line"]);
  cube = object_cast<Mesh3DPtr>(globals(*interpreter)["cube"]);
  return true;
}

bool MeshTest::main()
{
  ctx->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // draw 3D
  ctx->camera(camera3D);
  ctx->depthTest(true);
  ctx->draw(cube);
  // draw 2D over 3D scene
  ctx->camera(camera2D);
  ctx->depthTest(false);
  ctx->draw(quad);
  ctx->draw(line);
  
  ctx->swapBuffers();   
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
