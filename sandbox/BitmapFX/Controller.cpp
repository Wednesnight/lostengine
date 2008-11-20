#include "Controller.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/math/Vec2.h"
#include "lost/math/Rect.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/application/Application.h"
#include "lost/application/KeySym.h"

#include "lost/font/freetype/Library.h"
#include "lost/font/freetype/Face.h"
#include "lost/bitmap/Packer.h"

using namespace std;
using namespace boost;
using namespace lost;
using namespace lost::bitmap;
using namespace lost::font;
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

  shared_ptr<freetype::Library> ftlib(new freetype::Library);
  shared_ptr<File> file = appInstance->loader->load("suigeneris.ttf");
  shared_ptr<freetype::Face> fnt(new freetype::Face(ftlib, file));
  ttf.reset(new TrueTypeFont(ftlib, file));  
    ttf->atlasSize = Vec2(128,128);
  fontSize = appInstance->config["fontSize"].as<uint32_t>();
  string text ="Hoschi!gnjVAfiglrby";
  renderedText1 = ttf->render(text, 16);
  renderedText2 = ttf->render(text, 24);
  renderedText3 = ttf->render(text, 32);
    
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
  context->set2DProjection(Vec2(0, 0), Vec2(appInstance->displayAttributes->width, appInstance->displayAttributes->height));
  context->pushState(renderState);
  context->clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  context->setColor(whiteColor);
  context->drawRectTextured(Rect(300,0,ttf->atlas->width, ttf->atlas->height), ttf->atlas, false);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0,10,0);
  context->setColor(redColor);
  renderedText1->render(context);
  glTranslatef(0,30,0);
  context->setColor(Color(0,1,0,0.75));
  renderedText2->render(context);
  glTranslatef(0,60,0);
  context->setColor(Color(0,0,1,0.5));
  renderedText3->render(context);
  context->popState();

  fpsMeter->render( appInstance->displayAttributes->width - (fpsMeter->width + 10), 0, event->passedSec );
  appInstance->swapBuffers();
}
