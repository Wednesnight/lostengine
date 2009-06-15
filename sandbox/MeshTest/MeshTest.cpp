#include "MeshTest.h"
#include "lost/event/Receive.h"
#include "lost/common/Logger.h"
#include "lost/application/ApplicationEvent.h"

#include "lost/rg/Draw.h"
#include "lost/rg/DepthTest.h"
#include "lost/rg/Camera.h"

#include "lost/math/Bezier.h"
#include "lost/mesh/Rect.h"

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
  animate = true;
}


bool MeshTest::startup()
{
  bool result = Tasklet::startup();
  if(!result)
  {
    EOUT("Tasklet::startup() failed, aborting");
    return false;
  }
  // key handlers
  eventDispatcher->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&MeshTest::keyDownHandler, this, _1)));

  meter.reset(new Meter());
  lua->globals["meter"] = MeshPtr(meter->mesh);

  std::vector<Rect> rects;
  float s = 10;
  for(uint32_t i=0; i<10; ++i)
  {
    rects.push_back(Rect(i*s*1.5, 13, s, s));
  }
  Quad2DPtr quads = Quad2D::create(rects);

  scene = lua->globals["scene"]; // required for drawing
  threedScene = lua->globals["threedScene"];
  textScene = lua->globals["textScene"];
  rg::DrawPtr cubeDrawNode = static_pointer_cast<rg::Draw>(scene->recursiveFindByName("cube"));
  if(cubeDrawNode) // required for updates
  {
    cube = cubeDrawNode->mesh;
    // disabled for the moment
    cubeDrawNode->active = false;
  }
  else
  {
    EOUT("CUBE NOT FOUND !!!!");
  }
  rg::NodePtr bg = scene->recursiveFindByName("2D Background");
  bg->add(rg::Draw::create(quads));
  MeshPtr testLine = Line2D::create(Vec2(0,0), Vec2(639,479));
  testLine->material->color = common::greenColor;
  bg->add(rg::Draw::create(testLine));

  /**
   linear bezier curve including control points
   */
  MeshPtr line = Line2D::create(Vec2(110,100), Vec2(120,150));
  line->material->color = common::redColor;
  bg->add(rg::Draw::create(line));
  LinearBezierPtr linearBezier = LinearBezier::create(Vec2(110,100), Vec2(120, 150));
  bg->add(rg::Draw::create(Line2D::create(linearBezier->points)));

  /**
   quadratic bezier curve including control points
   */
  line = Line2D::create(Vec2(160,100), Vec2(180,150));
  line->material->color = common::redColor;
  bg->add(rg::Draw::create(line));
  line = Line2D::create(Vec2(180,150), Vec2(190,90));
  line->material->color = common::redColor;
  bg->add(rg::Draw::create(line));
  QuadraticBezierPtr quadraticBezier = QuadraticBezier::create(Vec2(160,100), Vec2(180, 150), Vec2(190, 90));
  bg->add(rg::Draw::create(Line2D::create(quadraticBezier->points)));

  /**
   cubic bezier curve including control points
   */
  line = Line2D::create(Vec2(210,100), Vec2(220,150));
  line->material->color = common::redColor;
  bg->add(rg::Draw::create(line));
  line = Line2D::create(Vec2(230,90), Vec2(245,120));
  line->material->color = common::redColor;
  bg->add(rg::Draw::create(line));
  CubicBezierPtr cubicBezier = CubicBezier::create(Vec2(210,100), Vec2(220, 150), Vec2(230, 90), Vec2(245, 120));
  bg->add(rg::Draw::create(Line2D::create(cubicBezier->points)));

  /**
   standardized bezier curve, used to define rounded corners
   */
  Rect rect(300, 50, 100, 100);
  QuadraticBezierPtr roundCorner = QuadraticBezier::create(rect.bottomLeft(), Vec2(1,1), 25);
  bg->add(rg::Draw::create(Line2D::create(roundCorner->points)));
  roundCorner->update(rect.bottomRight(), Vec2(-1,1), 25);
  bg->add(rg::Draw::create(Line2D::create(roundCorner->points)));
  roundCorner->update(rect.topRight(), Vec2(-1,-1), 25);
  bg->add(rg::Draw::create(Line2D::create(roundCorner->points)));
  roundCorner->update(rect.topLeft(), Vec2(1,-1), 25);
  bg->add(rg::Draw::create(Line2D::create(roundCorner->points)));

  /**
   rect
   */
  Rect2DPtr rectMesh = Rect2D::create(Rect(450, 50, 100, 100));
  rectMesh->material->color = common::greenColor;
  bg->add(rg::Draw::create(rectMesh));

  /**
   filled rect
   */
  FilledRect2DPtr filledRectMesh = FilledRect2D::create(Rect(450, 200, 100, 100));
  filledRectMesh->material->color = common::greenColor;
  bg->add(rg::Draw::create(filledRectMesh));
  
  /**
   rounded rect
   */
  RoundedRect2DPtr roundedRectMesh = RoundedRect2D::create(Rect(300, 200, 100, 100), 25, 25);
  roundedRectMesh->material->color = common::greenColor;
  bg->add(rg::Draw::create(roundedRectMesh));
  
  /**
   filled rounded rect
   */
  FilledRoundedRect2DPtr filledRoundedRectMesh = FilledRoundedRect2D::create(Rect(150, 200, 100, 100), 25, 25);
  filledRoundedRectMesh->material->color = common::greenColor;
  bg->add(rg::Draw::create(filledRoundedRectMesh));
  
  std::vector<Vec2> v;
  v.push_back(Vec2(10,100));
  v.push_back(Vec2(20,150));
  v.push_back(Vec2(30,90));
  v.push_back(Vec2(45,400));
  v.push_back(Vec2(47,190));
  MeshPtr multiLines = Line2D::create(v);
  multiLines->material->color = common::yellowColor;
  bg->add(rg::Draw::create(multiLines));

  activeScene = scene;

  return result;
}

void MeshTest::update(double dt)
{
  if (animate)
  {
    angle = fmod(dt*50+angle, 360);
    if(cube)
    {
      cube->modelTransform = MatrixRotX(angle) * MatrixRotY(angle);
    }
  }
}

void MeshTest::draw()
{
  if(activeScene)
    activeScene->process(window->context);
  else
    DOUT("scene is NULL!");
  window->context->swapBuffers();  
}

bool MeshTest::main()
{
  double currentSec = lost::platform::currentTimeSeconds();
  double delta = currentSec - passedSec;
  update(delta);
  draw();
    
//  meter->update(1.0f/delta);
  passedSec = currentSec; 
  return true;
}

bool MeshTest::shutdown()
{
  return true;
}

void MeshTest::keyDownHandler(KeyEventPtr event)
{
  if (event->key == K_ESCAPE) 
    eventDispatcher->dispatchEvent(ApplicationEventPtr(new ApplicationEvent(ApplicationEvent::QUIT())));

  switch(event->key)
  {
    case K_1:activeScene = scene;break;
    case K_2:activeScene = threedScene;break;
    case K_3:activeScene = textScene;break;
    case K_SPACE:animate = !animate;break;
  }
  if (event->key != K_SPACE) animate = activeScene == scene;
}
