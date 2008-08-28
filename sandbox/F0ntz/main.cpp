#include "lost/gl/gl.h"
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "lost/common/Logger.h"
#include "lost/common/FpsMeter.h"
#include "lost/gl/Utils.h"
#include "lost/math/Vec2.h"
#include "lost/math/Rect.h"
#include "lost/gl/Draw.h"
#include "lost/font/freetype/Library.h"
#include "lost/font/freetype/Face.h"
#include "lost/font/freetype/Glyph.h"
#include "lost/font/freetype/Renderer.h"
#include "lost/gl/DisplayList.h"
#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/Timer.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/KeySym.h"
#include "lost/resource/DefaultLoader.h"

using namespace std;
using namespace boost;
using namespace lost;
using namespace lost::common;
using namespace lost::resource;
using namespace lost::application;
using namespace lost::math;
using namespace lost::event;
using namespace lost::font::freetype;

struct Controller 
{
  DisplayAttributes   display;
  FpsMeter            fpsmeter;
  shared_ptr<Library> freetypeLibrary;
  shared_ptr<Face>  defaultFont;
  shared_ptr<Renderer>  renderer;
  shared_ptr<gl::Texture> glyphtex;
//  shared_ptr<lost::font::freetype::Glyph> glyphquad;
  shared_ptr<lost::gl::DisplayList> smallText;
  shared_ptr<lost::gl::DisplayList> midText;
  shared_ptr<lost::gl::DisplayList> largeText;
  shared_ptr<File>  fontfile;
  
  double passedTime;

  Controller()
  {
    passedTime = 0;
  }

  void init(shared_ptr<Event> event)
  {
//    IOUT("driver reports OpenGL version " << gl::utils::getVersion());
//    IOUT("Extensions: "<<glGetString(GL_EXTENSIONS));


    freetypeLibrary.reset(new Library());
    renderer.reset(new Renderer);
    fontfile = appInstance->loader->load("Vera.ttf");
    defaultFont = freetypeLibrary->initFace(fontfile);
//    unsigned long fontSizeInPoints = 164;
//    glyphquad = defaultFont->getGlyph(fontSizeInPoints, 'S');
    smallText.reset(new lost::gl::DisplayList());
    midText.reset(new lost::gl::DisplayList());
    largeText.reset(new lost::gl::DisplayList());

    renderer->renderText(defaultFont, 10, "The quick brown fox jumps over the lazy dog VA fi glrb", smallText);
//    defaultFont->renderText(16, "The quick brown fox jumps over the lazy dog VA fi glrb", smallText);
    renderer->renderText(defaultFont, 64, "The quick brown fox jumps over the lazy dog VA fi glrb", midText);
    renderer->renderText(defaultFont, 164, "The quick brown fox jumps over the lazy dog VA fi glrb", largeText);

  }

  void keyboard( shared_ptr<KeyEvent> event )
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

  void redraw(shared_ptr<TimerEvent> event)
  {
    //glEnable( GL_SCISSOR_TEST ); FIXME: leave this off or inital scissor will fuck up ui on window resize, we need to investigate this
    GLDEBUG;
    glEnable(GL_BLEND);GLDEBUG;
    glDisable(GL_DEPTH_TEST);GLDEBUG;
    glDisable(GL_TEXTURE_2D);GLDEBUG;
    glClearColor( 0.0, 0.0, 0.0, 0.0 );GLDEBUG;
    glViewport(0, 0, display.width, display.height);GLDEBUG;
    lost::gl::utils::set2DProjection(lost::math::Vec2(0, 0), lost::math::Vec2(appInstance->displayAttributes.width, appInstance->displayAttributes.height));GLDEBUG;
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
    fpsmeter.render( appInstance->displayAttributes.width - (fpsmeter.width + 10), 0, event->passedSec );
    glDisableClientState(GL_VERTEX_ARRAY);

    appInstance->swapBuffers();
  }
};

int main( int argc, char *argv[] )
{
  LogLevel( log_all );

  try
  {
    Application app;
    Controller  controller;
    Timer       redrawTimer("redraw", 1.0/60.0); 

    redrawTimer.addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&Controller::redraw, &controller, _1)));
    app.addEventListener(ApplicationEvent::INIT(), bind(&Controller::init, &controller, _1));
    app.addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Controller::keyboard, &controller, _1)));
    app.addEventListener(KeyEvent::KEY_UP(), receive<KeyEvent>(bind(&Controller::keyboard, &controller, _1)));

    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return EXIT_SUCCESS;
}
