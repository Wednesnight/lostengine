#include <iostream>

#include <boost/serialization/shared_ptr.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "lost/common/Logger.h"
#include "lost/platform/Platform.h"

#include "lost/object/Object.h"
#include "lost/object/ObjectReference.h"
#include "lost/object/ObjectReferenceContainer.h"
#include "lost/object/ObjectReferenceResolver.h"

#include "lost/guiro/factory/Factory.h"

#include "lost/guiro/factory/VanillaFactory.h"

#include "lost/engine/glfw/Engine.h"
#include "lost/common/FpsMeter.h"
#include "lost/gl/Utils.h"

#include "lost/guiro/io/Io.h"

#include "lost/event/KeyEvent.h"
#include "lost/engine/KeySym.h"

#include "lost/math/Vec2.h"

#include "lost/guiro/control/Screen.h"
#include "lost/guiro/control/Slider.h"
#include "lost/guiro/control/UserInterface.h"
#include "lost/common/Config.h"

#include "VanillaUIBuilder.h"
#include "FuglyUIBuilder.h"

// FIXME: we need this on linux
#include "lost/gl/gl.h"

using namespace std;
using namespace boost;
using namespace lost;
using namespace lost::guiro;
using namespace lost::guiro::control;
using namespace lost::guiro::common;
using namespace lost::guiro::factory;
using namespace lost::object;
using namespace lost::common;
using namespace lost::math;
using namespace lost::engine;

namespace tech = engine::glfw;

struct LostEngine : tech::Engine
{
    DisplayAttributes   display;
    FpsMeter            fpsmeter;
    shared_ptr<Screen>  screen;
    VanillaUIBuilder    vanillaUIbuilder;
    FuglyUIBuilder      fuglyUIbuilder;

    LostEngine( lost::shared_ptr<Config> inConfig )
      : tech::Engine::Engine(inConfig),
        display(*inConfig),
        vanillaUIbuilder(inConfig),
        fuglyUIbuilder(inConfig)
    {
    }

    void buildUI()
    {
      screen.reset(new Screen(display));
      screen->add(vanillaUIbuilder.build("vanillaUI"));
      screen->add(fuglyUIbuilder.build("fuglyUI"));
      screen->updateGeometry(); // this one is required to get the initial layout right.
      screen->get<Slider>("vanillaUI/scrollView/slider")->valueChange.connect( boost::bind( &LostEngine::sliderValueChange, this, _1 ) );
      screen->get<Slider>("vanillaUI/scrollView/steppedSlider")->valueChange.connect( boost::bind( &LostEngine::sliderValueChange, this, _1 ) );
      screen->get<Slider>("vanillaUI/scrollView/sliderVertical")->valueChange.connect( boost::bind( &LostEngine::sliderValueChange, this, _1 ) );
      screen->get<Slider>("vanillaUI/scrollView/steppedSliderVertical")->valueChange.connect( boost::bind( &LostEngine::sliderValueChange, this, _1 ) );
      screen->get<Scrollbar>("vanillaUI/scrollView/scrollbar")->positionChanged.connect( boost::bind( &LostEngine::scrollbarPositionChanged, this, _1 ) );
      DOUT("\n" << screen->getViewHierarchyAsString());
    }

    void run()
    {
      video.restart(display);
      video.setWindowTitle("RefTest");
      // if "resolve" triggers any OpenGL functions, it MUST ONLY be called AFTER OpenGL was initialised
      // so: restart display, resolve all textures or DIE! HORRIBLY!
      // that's why we build the UI here
      buildUI();

      IOUT("driver reports OpenGL version " << gl::utils::getVersion());

      timer.setTimerResolution(0.001);
      timer.add("redraw", 0.015);
      timer["redraw"].connect(boost::bind(&LostEngine::redraw, this, _1));

      // handle keyboard input
      input.keyboard.connect( boost::bind( &LostEngine::keyboard, this, _1 ) );
      // connect InputManager events
      input.keyboard.connect( boost::bind( &Screen::injectKeyboardEvent, screen, _1 ) );
      input.mouseMove.connect( boost::bind( &Screen::injectMouseMoveEvent, screen, _1 ) );
      input.mouseButton.connect( boost::bind( &Screen::injectMouseButtonEvent, screen, _1 ) );

      video.windowResize.connect( boost::bind( &LostEngine::windowResize, this, _1, _2 ) );

      // run the loop, tiers and redraws
      tech::Engine::run();
    }

  void keyboard( const lost::event::KeyEvent& inEvent )
    {
      switch (inEvent.key)
      {
        case K_ESCAPE :
          if (!inEvent.pressed) shutdown();
          break;
        case K_SPACE :
          if (!inEvent.pressed)
          {
            bool visible = (*screen)["vanillaUI"]->getVisible();
            (*screen)["vanillaUI"]->setVisible(!visible);
            DOUT("UserInterface visible: " << !visible);
          }
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
      screen->updateGeometry();
    }

    void resetViewPort()
    {
      glViewport(0, 0, display.width, display.height);
      GLDEBUG;
      lost::gl::utils::set2DProjection(lost::math::Vec2(0, 0), lost::math::Vec2(display.width, display.height));
      GLDEBUG;
    }

    void redraw(double timeSinceLastCallSec)
    {
      //glEnable( GL_SCISSOR_TEST ); FIXME: leave this off or inital scissor will fuck up ui on window resize, we need to investigate this
      GLDEBUG;
      glEnable(GL_BLEND);
      GLDEBUG;
      glDisable(GL_DEPTH_TEST);
      GLDEBUG;
      glDisable(GL_TEXTURE_2D);
      GLDEBUG;
      glClearColor( 0.0, 0.0, 0.0, 0.0 );
      GLDEBUG;
      resetViewPort();
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      GLDEBUG;

      screen->update( timeSinceLastCallSec );
      screen->draw();
      fpsmeter.render( display.width - (fpsmeter.width + 10), 0, timeSinceLastCallSec );
      video.swapBuffers();
    }

    void sliderValueChange( boost::any& sender )
    {
      Slider* slider = boost::any_cast<Slider*>(sender);
      if (slider)
      {
        DOUT( "sliderValueChange: slider \""+ slider->name +"\" has value " << slider->value() );
      }
    }

    void scrollbarPositionChanged( boost::any& sender )
    {
      Scrollbar* scrollbar = boost::any_cast<Scrollbar*>(sender);
      if (scrollbar)
      {
        DOUT( "scrollbarPositionChanged: scrollbar \""+ scrollbar->name +"\" is at " << scrollbar->position() * 100 << "%" );
      }
    }

};

int main( int argc, char *argv[] )
{
  // FIXME: we need this on linux
  glutInit( &argc, argv );

  LogLevel( log_all );

  try
  {
    shared_ptr<Config> config(new Config( argc, argv, lost::platform::buildResourcePath( "LostEngine.cfg" ) ));
    LostEngine         engine(config);

    engine.init();
    engine.run();
    config->saveToFile( lost::platform::buildResourcePath( "LostEngine.cfg" ) );
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return EXIT_SUCCESS;
}
