#include "NativeTasklet.h"
#include "lost/application/WindowParams.h"
#include "lost/math/Rect.h"
#include "lost/common/Color.h"
#include "lost/rg/ClearColor.h"
#include "lost/rg/Clear.h"
#include "lost/gl/gl.h"
#include "lost/event/Receive.h"
#include "lost/application/KeyCode.h"

using namespace lost;
using namespace lost::application;
using namespace lost::common;
using namespace lost::event;
using namespace lost::math;
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

NativeTasklet::NativeTasklet()
{
  name = "NativeTasklet Demo";
  waitForEvents = true;
}

void NativeTasklet::init()
{
  MultiThreadedTasklet::init();
  // setup window
  WindowParams params("NativeTasklet Demo", Rect(100,100,640,480));
  createWindow(params);

  // setup event listeners
  window->dispatcher->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&NativeTasklet::keyDown, this, _1)));

  Tasklet::init();
}

bool NativeTasklet::startup()
{
  if(!MultiThreadedTasklet::startup())
    return false;
  
  ClearColorPtr clearColor = ClearColor::create(blackColor);
  ClearPtr clear = Clear::create(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderNode->add(clearColor);
  renderNode->add(clear);

  running = true;

  return running;
}

bool NativeTasklet::update()
{
  return (running && MultiThreadedTasklet::update());
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
