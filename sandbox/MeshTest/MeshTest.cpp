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
using namespace lost::font;
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

  // initialize font
  ttf = TrueTypeFontPtr(new TrueTypeFont(font::freetype::Library::create(), loader->load("Vera.ttf")));

  meter.reset(new Meter());
  lua->globals["meter"] = MeshPtr(meter->mesh);

  std::vector<Rect> rects;
  float s = 10;
  for(uint32_t i=0; i<10; ++i)
  {
    rects.push_back(Rect(i*s*1.5, 13, s, s));
  }
  Quad2DPtr quads = Quad2D::create(rects);

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
    rg::NodePtr bg = scene->recursiveFindByName("2D Background");
    bg->add(rg::Draw::create(quads));
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
