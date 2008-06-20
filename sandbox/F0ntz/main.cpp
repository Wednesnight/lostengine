#include "lost/gl/gl.h"
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "lost/common/Logger.h"
#include "lost/Platform/Platform.h"
#include "lost/engine/glfw/Engine.h"
#include "lost/common/FpsMeter.h"
#include "lost/gl/Utils.h"
#include "lost/event/KeyEvent.h"
#include "lost/engine/KeySym.h"
#include "lost/math/Vec2.h"
#include "lost/math/Rect.h"
#include "lost/common/Config.h"
#include "lost/gl/Draw.h"

#include "lost/font/freetype/Library.h"
#include "lost/font/freetype/Face.h"
#include "lost/font/freetype/Glyph.h"

#include "lost/gl/DisplayList.h"

using namespace std;
using namespace boost;
using namespace lost;
using namespace lost::common;
using namespace lost::math;
using namespace lost::engine;
using namespace lost::font::freetype;

namespace tech = engine::glfw;

struct LostEngine : tech::Engine
{
  DisplayAttributes   display;
  FpsMeter            fpsmeter;
  shared_ptr<Library> freetypeLibrary;
  shared_ptr<Face>  defaultFont;
  shared_ptr<gl::Texture> glyphtex;
  shared_ptr<lost::font::freetype::Glyph> glyphquad;
  shared_ptr<lost::gl::DisplayList> smallText;
  shared_ptr<lost::gl::DisplayList> midText;
  shared_ptr<lost::gl::DisplayList> largeText;

  double passedTime;

  LostEngine( boost::shared_ptr<Config> inConfig )
  : tech::Engine::Engine(inConfig),
  display(*inConfig)
  {
    passedTime = 0;
  }

  void fontit()
  {
    freetypeLibrary.reset(new Library());

    string path = lost::platform::buildResourcePath( "Vera.ttf" );
    defaultFont = freetypeLibrary->initFaceFromFile(path);
    unsigned long fontSizeInPoints = 164;
    glyphquad = defaultFont->getGlyph(fontSizeInPoints, 'S');
    smallText.reset(new lost::gl::DisplayList());
    midText.reset(new lost::gl::DisplayList());
    largeText.reset(new lost::gl::DisplayList());

    defaultFont->renderText(10, "The quick brown fox jumps over the lazy dog VA fi glrb", smallText);
//    defaultFont->renderText(16, "The quick brown fox jumps over the lazy dog VA fi glrb", smallText);
    defaultFont->renderText(64, "The quick brown fox jumps over the lazy dog VA fi glrb", midText);
    defaultFont->renderText(164, "The quick brown fox jumps over the lazy dog VA fi glrb", largeText);
  }

  void run()
  {
    video.restart(display);
    video.setWindowTitle("F0ntz");

    IOUT("driver reports OpenGL version " << gl::utils::getVersion());
    IOUT("Extensions: "<<glGetString(GL_EXTENSIONS));

    timer.setTimerResolution(0.001);
    timer.add("redraw", 0.015);
    timer["redraw"].connect(boost::bind(&LostEngine::redraw, this, _1));

    // handle keyboard input
    input.keyboard.connect( boost::bind( &LostEngine::keyboard, this, _1 ) );
    // connect InputManager events
    input.mouseMove.connect( boost::bind( &LostEngine::injectMouseMoveEvent, this, _1 ) );
    input.mouseButton.connect( boost::bind( &LostEngine::injectMouseButtonEvent, this, _1 ) );

    video.windowResize.connect( boost::bind( &LostEngine::windowResize, this, _1, _2 ) );

    fontit();

    // run the loop, timers and redraws
    tech::Engine::run();
  }

  void injectMouseButtonEvent( const lost::event::MouseEvent& inEvent )
  {
  }

  void injectMouseMoveEvent( const lost::event::MouseEvent& inEvent )
  {
  }

  void keyboard( const lost::event::KeyEvent& inEvent )
  {
    switch (inEvent.key)
    {
      case K_ESCAPE :
        if (!inEvent.pressed) shutdown();
        break;
        case K_SPACE :
        break;
        default :
        break;
    }
  }

  void windowResize(int width, int height)
  {
    DOUT("window resize w:"<<width<<" h:"<<height);
    display.width = width;
    display.height = height;
    video.attributes = display; // FIXME: stupid ugly fucking hack!! the whole engine needs a DisplayAttributes singleton thats accessed from everywhere
    // but since it contains its own copy now, we need to update it somewhere.
  }

  void resetViewPort()
  {
    glViewport(0, 0, display.width, display.height);GLDEBUG;
    lost::gl::utils::set2DProjection(lost::math::Vec2(0, 0), lost::math::Vec2(display.width, display.height));GLDEBUG;
  }

  void redraw(double timeSinceLastCallSec)
  {
    //glEnable( GL_SCISSOR_TEST ); FIXME: leave this off or inital scissor will fuck up ui on window resize, we need to investigate this
    GLDEBUG;
    glEnable(GL_BLEND);GLDEBUG;
    glDisable(GL_DEPTH_TEST);GLDEBUG;
    glDisable(GL_TEXTURE_2D);GLDEBUG;
    glClearColor( 0.0, 0.0, 0.0, 0.0 );GLDEBUG;
    resetViewPort();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); GLDEBUG;
    glEnable(GL_TEXTURE_2D);GLDEBUG;
    glColor4f(1, 1, 1, 1);GLDEBUG;

    // translate the rendered string in the displaylist without destroying the original modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    passedTime += timeSinceLastCallSec;
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

    fpsmeter.render( display.width - (fpsmeter.width + 10), 0, timeSinceLastCallSec );
    video.swapBuffers();
  }
};

int main( int argc, char *argv[] )
{
  LogLevel( log_all );

  try
  {
    shared_ptr<Config> config(new Config( argc, argv, lost::platform::buildResourcePath( "F0ntz.cfg" ) ));
    LostEngine         engine(config);

    engine.init();
    engine.run();
    config->saveToFile( lost::platform::buildResourcePath( "F0ntz.cfg" ) );
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return EXIT_SUCCESS;
}
