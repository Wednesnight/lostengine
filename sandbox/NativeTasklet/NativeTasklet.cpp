#include "NativeTasklet.h"
#include "lost/application/WindowParams.h"
#include "lost/math/Rect.h"
#include "lost/common/Color.h"
#include "lost/rg/ClearColor.h"
#include "lost/rg/Clear.h"
#include "lost/gl/gl.h"
#include "lost/event/Receive.h"
#include "lost/application/KeyCode.h"
#include "lost/event/EventDispatcher.h"

#include "lost/camera/Camera2D.h"
#include "lost/rg/DepthTest.h"
#include "lost/rg/Camera.h"
#include "lost/rg/Draw.h"
#include "lost/mesh/Line.h"
#include "lost/math/Vec2.h"
#include "lost/gl/ShaderHelper.h"

#include <boost/bind.hpp>

using namespace lost;
using namespace lost::application;
using namespace lost::camera;
using namespace lost::common;
using namespace lost::event;
using namespace lost::gl;
using namespace lost::math;
using namespace lost::mesh;
using namespace lost::rg;

void ToggleClearColor::process(const lost::application::Tasklet* sender) const
{
  NodePtr node = sender->renderNode->recursiveFindByName("clearColor");
  if (node)
  {
    ClearColorPtr clearColor = lost::dynamic_pointer_cast<ClearColor>(node);
    if (clearColor->col == blackColor)
    {
      clearColor->col = whiteColor;
    }
    else
    {
      clearColor->col = blackColor;
    }
  }
}

NativeTasklet::NativeTasklet() {

  config.taskletName = "NativeTasklet Demo";
  config.taskletWaitForEvents = false;
  config.taskletHasWindow = true;
  
  config.windowTitle = config.taskletName;
  config.windowRect = Rect(100, 100, 640, 480);
}

bool NativeTasklet::startup()
{
  if(!Tasklet::startup())
    return false;
  
  // setup event listeners
  window->dispatcher->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&NativeTasklet::keyDown, this, _1)));

  // setup nodes
  ClearColorPtr clearColor = ClearColor::create(blackColor);
  ClearPtr clear = Clear::create(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Camera2DPtr camera = Camera2D::create(Rect(0, 0, config.windowRect.width, config.windowRect.height));
  rg::CameraPtr scene2D = rg::Camera::create(camera);

  scene2D->add(clearColor);
  scene2D->add(clear);
  scene2D->add(DepthTest::create(false));

  scene2D->add(createLine(Vec2(10, 10), Vec2(100, 100), 1, redColor));
  scene2D->add(createLine(Vec2(150, 10), Vec2(150, 100), 1, greenColor));
  scene2D->add(createLine(Vec2(160, 10), Vec2(250, 10), 1, blueColor));
  scene2D->add(createLine(Vec2(360, 10), Vec2(260, 100), 1, yellowColor));
  scene2D->add(createLine(Vec2(460, 100), Vec2(380, 10), 1, Color(1.0f, 0.0f, 1.0f, 1.0f)));

  scene2D->add(createLine(Vec2(10, 110), Vec2(100, 200), 2, redColor));
  scene2D->add(createLine(Vec2(150, 110), Vec2(150, 200), 2, greenColor));
  scene2D->add(createLine(Vec2(160, 110), Vec2(250, 110), 2, blueColor));
  scene2D->add(createLine(Vec2(360, 110), Vec2(260, 200), 2, yellowColor));
  scene2D->add(createLine(Vec2(460, 200), Vec2(380, 110), 2, Color(1.0f, 0.0f, 1.0f, 1.0f)));
  
  scene2D->add(createLine(Vec2(10, 210), Vec2(100, 300), 5, redColor));
  scene2D->add(createLine(Vec2(150, 210), Vec2(150, 300), 5, greenColor));
  scene2D->add(createLine(Vec2(160, 210), Vec2(250, 210), 5, blueColor));
  scene2D->add(createLine(Vec2(360, 210), Vec2(260, 300), 5, yellowColor));
  scene2D->add(createLine(Vec2(460, 300), Vec2(380, 210), 5, Color(1.0f, 0.0f, 1.0f, 1.0f)));
  
  scene2D->add(createLine(Vec2(10, 310), Vec2(100, 400), 20, redColor));
  scene2D->add(createLine(Vec2(150, 310), Vec2(150, 400), 20, greenColor));
  scene2D->add(createLine(Vec2(160, 310), Vec2(250, 310), 20, blueColor));
  scene2D->add(createLine(Vec2(360, 310), Vec2(260, 400), 20, yellowColor));
  scene2D->add(createLine(Vec2(460, 400), Vec2(380, 310), 20, Color(1.0f, 0.0f, 1.0f, 1.0f)));

  renderNode->add(scene2D);

  running = true;

  return running;
}

NodePtr NativeTasklet::createLine(const Vec2& start, const Vec2& finish, const int width, const Color& color = whiteColor) {

  if (!textureManager) {

    textureManager = TextureManager::create();
  }
  LinePtr line = Line::create(textureManager, start, finish, width);
  line->material->blendPremultiplied();
  line->material->color = color;
  line->material->shader = loadShader(loader, "basic_texture");
  return Draw::create(line);
}

bool NativeTasklet::update(double deltaSec)
{
  return (running && Tasklet::update(deltaSec));
}

void NativeTasklet::keyDown(KeyEventPtr event)
{
  switch (event->key)
  {
    case K_SPACE:
      updateQueue->queue(new ToggleClearColor());
      break;

    case K_ESCAPE:
      running = false;
      break;
  }
}
