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
using namespace lost::camera;
using namespace lost::mesh;


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
  // line
  line.reset(new mesh::Line2D);
  line->update(Vec2(0,0), Vec2(400,400));
  line->material.reset(new Material);
  line->material->color = yellowColor;
  // Quad
//  quad.reset(new Quad2D(loader->load("stubs.jpg")));
  quad.reset(new Quad2D(loader->load("zim.png")));
  // cube
  cube = lost::model::Loader::obj(loader, "cube_tri.obj");
  cube->material.reset(new Material);
  cube->material->color = greenColor;
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
  luabind::object func = luabind::globals(*interpreter)["initShaders"];
  cube->material->shader = luabind::call_function<ShaderProgramPtr>(func, loader);  
  return true;
}

void MeshTest::draw2D()
{
  ctx->camera(camera2D);
  ctx->depthTest(false);
  ctx->draw(quad);
  ctx->draw(line);
}

void MeshTest::draw3D()
{
  ctx->camera(camera3D);
  ctx->depthTest(true);
  ctx->draw(cube);
}

bool MeshTest::main()
{
  ctx->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  try
  {
    draw3D();
    draw2D();
  }
  catch(std::exception& ex)
  {
    EOUT("caught error: "<<ex.what());
  }
  
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
