#include "MeshTest.h"
#include "lost/event/Receive.h"
#include "lost/common/Logger.h"
#include "lost/application/ApplicationEvent.h"

#include "lost/rg/Draw.h"
#include "lost/rg/DepthTest.h"
#include "lost/rg/Camera.h"

#include "lost/gl/TextureAtlas.h"
#include "lost/math/Bezier.h"
#include "lost/mesh/Rect.h"
#include "lost/mesh/Axes.h"
#include "lost/mesh/ScaleGrid.h"
#include "lost/mesh/Circular.h"
#include "lost/mesh/AABB.h"

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

const static Vec2 screenSize(640,480);

MeshTest::MeshTest()
: UiTasklet(WindowParams("MeshTest", Rect(50,200,screenSize.width, screenSize.height)))
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

  // build the display
  vector<string> titles;
  titles.push_back("Primitives");
  titles.push_back("3D");
  titles.push_back("Text");
  titles.push_back("Pixel Perfect");

  vector<string> descriptions;
  descriptions.push_back("tests various 2D building blocks");
  descriptions.push_back("tests various 3D meshes");
  descriptions.push_back("test text and atlas for all text renders");
  descriptions.push_back("test pixel perfect drawing");

  TrueTypeFontPtr fnt = lua->globals["verattf"];
  selectionDisplay.reset(new SelectionDisplay(fnt));
  selectionDisplay->build(screenSize, titles, descriptions);

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
  tunaScene = lua->globals["tunaScene"];
  pixelperfectScene = lua->globals["pixelperfectScene"];
  rg::DrawPtr cubeDrawNode = static_pointer_cast<rg::Draw>(threedScene->recursiveFindByName("cube"));
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
  MeshPtr testLine = Line2D::create(Vec2(0,0), Vec2(639,479));
  testLine->material->color = common::greenColor;
  bg->add(rg::Draw::create(testLine));

  /**
   linear bezier curve including control points
   */
  MeshPtr line = Line2D::create(Vec2(110,100), Vec2(120,150));
  line->material->color = common::redColor;
  bg->add(rg::Draw::create(line));
  LinearBezier2DPtr linearBezier = LinearBezier2D::create(Vec2(110,100), Vec2(120, 150));
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
  QuadraticBezier2DPtr quadraticBezier = QuadraticBezier2D::create(Vec2(160,100), Vec2(180, 150), Vec2(190, 90));
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
  CubicBezier2DPtr cubicBezier = CubicBezier2D::create(Vec2(210,100), Vec2(220, 150), Vec2(230, 90), Vec2(245, 120));
  bg->add(rg::Draw::create(Line2D::create(cubicBezier->points)));

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
  
  /**
   circle
   */
  Circle2DPtr circleMesh = Circle2D::create(25);
  circleMesh->material->color = common::redColor;
  circleMesh->modelTransform = MatrixTranslation(Vec3(450, 350, 0));
  bg->add(rg::Draw::create(circleMesh));
  
  /**
   filled circle
   */
  FilledCircle2DPtr filledCircleMesh = FilledCircle2D::create(25);
  filledCircleMesh->material->color = common::redColor;
  filledCircleMesh->modelTransform = MatrixTranslation(Vec3(375, 350, 0));
  bg->add(rg::Draw::create(filledCircleMesh));
  
  /**
   ellipse
   */
  Ellipse2DPtr ellipseMesh = Ellipse2D::create(Vec2(15,50));
  ellipseMesh->material->color = common::redColor;
  ellipseMesh->modelTransform = MatrixTranslation(Vec3(300, 350, 0));
  bg->add(rg::Draw::create(ellipseMesh));
  
  /**
   filled ellipse
   */
  FilledEllipse2DPtr filledEllipseMesh = FilledEllipse2D::create(Vec2(35, 15));
  filledEllipseMesh->material->color = common::redColor;
  filledEllipseMesh->modelTransform = MatrixTranslation(Vec3(225, 350, 0));
  bg->add(rg::Draw::create(filledEllipseMesh));
  
  /**
   Quad array
   */
  std::vector<Vec2> v;
  v.push_back(Vec2(10,100));
  v.push_back(Vec2(20,150));
  v.push_back(Vec2(30,90));
  v.push_back(Vec2(45,400));
  v.push_back(Vec2(47,190));
  MeshPtr multiLines = Line2D::create(v);
  multiLines->material->color = common::yellowColor;
  bg->add(rg::Draw::create(multiLines));

  /**
  Axes
   */
  axes = mesh::Axes3D::create();
  axes->material->blend = true;
  threedScene->add(rg::Draw::create(axes));

  /**
  AABB
   */
  aabb = mesh::AABB3D::create(math::AABB(Vec3(-.5f,-1,-.5f), Vec3(1,2,1)));
  threedScene->add(rg::Draw::create(aabb));  

  /**
  ScaleGrid
   */
  TexturePtr tex(new Texture(loader->load("defaultButton.png")));
  ScaleGrid2DPtr scaler = ScaleGrid2D::create(tex, math::Rect(20,20,100,100), 8, 48, 48,8);
  scaler->material->blend = false;
  bg->add(rg::Draw::create(scaler));

  TexturePtr tex2(new Texture(loader->load("scalegridTest.png")));
  ScaleGrid2DPtr scaler2 = ScaleGrid2D::create(tex2, math::Rect(150,20,40,40), 2, 17, 17,2);
  scaler->material->blend = false;
  bg->add(rg::Draw::create(scaler2));


  /// !!! 
  ///
  /// Add 3D objects to the scenes before adding the selection display or you'll
  /// end up trying to draw 3d, but the selectiondisplay 2D camera will still be active!
  ///
  /// !!!
  
  scene->add(selectionDisplay->rootNode);
  threedScene->add(selectionDisplay->rootNode);
  textScene->add(selectionDisplay->rootNode);
  pixelperfectScene->add(selectionDisplay->rootNode);
/*  TextureAtlasPtr textureAtlas = TextureAtlas::create();
  TexturePtr tunatex(new Texture(loader->load("tuna.png")));
  textureAtlas->tex = tunatex;*/

  activeScene = scene;
  selectionDisplay->highlight(0);
  return result;
}

void MeshTest::update(double dt)
{
  if (animate)
  {
    angle = fmod(dt*50+angle, 360);
    if(cube)
    {
      cube->modelTransform =   MatrixTranslation(Vec3(-3, 1, 0))
                             * MatrixRotX(angle)
                             * MatrixRotY(angle)
                             * MatrixTranslation(Vec3(-1, -1, -1));
    }
    if(axes)
    {
      axes->modelTransform = MatrixTranslation(Vec3(0, 1, 0))
                             * MatrixRotX(angle)
                             * MatrixRotY(angle);
    }

    if(aabb)
    {
      aabb->modelTransform = MatrixTranslation(Vec3(2, 1, 0))
                             * MatrixRotX(angle)
                             * MatrixRotY(angle);;
//                             * MatrixTranslation(Vec3(1, 0, 0));
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
    case K_1:activeScene = scene;selectionDisplay->highlight(0);break;
    case K_2:activeScene = threedScene; selectionDisplay->highlight(1);break;
    case K_3:activeScene = textScene; selectionDisplay->highlight(2);break;
    case K_4:activeScene = pixelperfectScene; selectionDisplay->highlight(3);break;
    case K_SPACE:animate = !animate;break;
  }
  if (event->key != K_SPACE) animate = activeScene == threedScene;
}
