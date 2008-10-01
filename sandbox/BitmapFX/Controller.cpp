#include "Controller.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/math/Vec2.h"
#include "lost/math/Rect.h"
#include "lost/gl/Draw.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/application/Application.h"
#include "lost/application/KeySym.h"

using namespace std;
using namespace boost;
using namespace lost;
using namespace lost::common;
using namespace lost::resource;
using namespace lost::application;
using namespace lost::math;
using namespace lost::event;
using namespace lost::gl;
using namespace lost::gl::utils;
using namespace lost::application;

Controller::Controller()
{
}

void Controller::init(shared_ptr<Event> event)
{
  context = appInstance->context;
  fpsMeter.reset(new FpsMeter(context));

  renderState = context->copyState();
  renderState->blend = true;
  renderState->depthTest = false;
  renderState->texture2D = false;
  renderState->clearColor = blackColor;
  renderState->vertexArray = false;  
}

void Controller::keyboard( shared_ptr<KeyEvent> event )
{
  switch (event->key)
  {
    case K_ESCAPE :
      if (!event->pressed) appInstance->quit();
      break;
      case K_SPACE :
      break;
      default :
      break;
  }
}

void Controller::redraw(shared_ptr<TimerEvent> event)
{
  context->pushState(renderState);
  glViewport(0, 0, display.width, display.height);GLDEBUG;
  set2DProjection(Vec2(0, 0), Vec2(appInstance->displayAttributes.width, appInstance->displayAttributes.height));
  context->clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  fpsMeter->render( appInstance->displayAttributes.width - (fpsMeter->width + 10), 0, event->passedSec );
  context->popState();
  appInstance->swapBuffers();
}
