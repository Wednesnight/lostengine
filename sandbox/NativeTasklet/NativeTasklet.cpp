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

#include <boost/bind.hpp>

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
  config.taskletName = "NativeTasklet Demo";
  config.taskletWaitForEvents = true;
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

  renderNode->add(clearColor);
  renderNode->add(clear);

  running = true;

  return running;
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
