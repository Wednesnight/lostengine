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
using namespace lost::bitmap;
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

  renderState = appInstance->context->copyState();
  renderState->texture2D = true;
  renderState->blend = true;
  renderState->blendSrc = GL_SRC_ALPHA;
  renderState->blendDest = GL_ONE_MINUS_SRC_ALPHA;
  renderState->clearColor = blackColor;
  renderState->depthTest = false;  
  renderState->alphaTest = false;  
  renderState->normalArray = false;  
  renderState->vertexArray = true;  
  renderState->textureCoordArray = true;  

  pic.reset(new Bitmap);
  pic->init(640, 480);
  
  shared_ptr<File> file = appInstance->loader->load(appInstance->config["bitmapFilename"].as<string>());
  loadedPic.reset(new Bitmap(file));
  tex.reset(new Texture);
  tex->bind();
  tex->reset(0, GL_RGB, false, loadedPic);
  tex->wrap(GL_CLAMP_TO_EDGE);
  tex->filter(GL_LINEAR);
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
  glViewport(0, 0, display.width, display.height);GLDEBUG;
  set2DProjection(Vec2(0, 0), Vec2(appInstance->displayAttributes.width, appInstance->displayAttributes.height));
  context->pushState(renderState);
  context->clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  setColor(whiteColor);
  drawRectTextured(Rect(0,0,tex->width, tex->height), tex);
  context->popState();

  fpsMeter->render( appInstance->displayAttributes.width - (fpsMeter->width + 10), 0, event->passedSec );
  appInstance->swapBuffers();
}
