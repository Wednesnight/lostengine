#include "Controller.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/math/Vec2.h"
#include "lost/math/Rect.h"
#include "lost/gl/DisplayList.h"
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
using namespace lost::font;
using namespace lost::gl::utils;
using namespace lost::font::freetype;
using namespace lost::application;

Controller::Controller()
{
  passedTime = 0;
}

void Controller::init(shared_ptr<Event> event)
{
  freetypeLibrary.reset(new Library());
  renderer.reset(new Renderer);
  shared_ptr<File> fontFile = appInstance->loader->load("Vera.ttf");
  defaultFont.reset(new Face(freetypeLibrary, fontFile));
  smallText.reset(new lost::gl::DisplayList());
  midText.reset(new lost::gl::DisplayList());
  largeText.reset(new lost::gl::DisplayList());

  fpsmeter.reset(new FpsMeter(appInstance->context));
  
  renderer->renderText(defaultFont, 10, "The quick brown fox jumps over the lazy dog VA fi glrb", smallText);
  renderer->renderText(defaultFont, 64, "The quick brown fox jumps over the lazy dog VA fi glrb", midText);
  renderer->renderText(defaultFont, 164, "The quick brown fox jumps over the lazy dog VA fi glrb", largeText);
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
  GLDEBUG;
  glEnable(GL_BLEND);GLDEBUG;
  glDisable(GL_DEPTH_TEST);GLDEBUG;
  glDisable(GL_TEXTURE_2D);GLDEBUG;
  glClearColor( 0.0, 0.0, 0.0, 0.0 );GLDEBUG;
  glViewport(0, 0, display.width, display.height);GLDEBUG;
  appInstance->context->set2DProjection(Vec2(0, 0), Vec2(appInstance->displayAttributes->width, appInstance->displayAttributes->height));GLDEBUG;
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); GLDEBUG;
  glEnable(GL_TEXTURE_2D);GLDEBUG;
  glColor4f(1, 1, 1, 1);GLDEBUG;

  // translate the rendered string in the displaylist without destroying the original modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  passedTime += event->passedSec;
  double tsin = sin(passedTime);
  double factor = 60;
  glTranslatef(0+tsin*factor, 40, 0);
  largeText->call();

  glTranslatef(0+tsin*factor, 164, 0);
  glColor4f(.6, .8, 1, 1);GLDEBUG;
  midText->call();

  glTranslatef(0+tsin*factor, 100, 0);
  glColor4f(.4, .6, .3, 1);GLDEBUG;
  smallText->call();

  glPopMatrix(); // restore the original modelview
  glDisable(GL_TEXTURE_2D);GLDEBUG;

  glEnableClientState(GL_VERTEX_ARRAY);
  fpsmeter->render( appInstance->displayAttributes->width - (fpsmeter->width + 10), 0, event->passedSec );
  glDisableClientState(GL_VERTEX_ARRAY);

  appInstance->swapBuffers();
}
