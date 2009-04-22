#include <iostream>
#include <string>
#include <math.h>

#include <boost/bind.hpp>

#include "lost/glfw/Engine.h"
#include "lost/gl/Utils.h"
#include "lost/Logger.h"
#include "Visual.h"
#include "StupidDots.h"
#include "Cock.h"
#include "MouseFuxx0r.h"
#include "Zombong.h"
#include "Pfonts.h"
#include "Imagombies.h"
#include "Guiro.h"
#include "lost/util/FpsMeter.h"
#include "lost/platform/Platform.h"

using namespace std;
using namespace lost;
using namespace lost::math;
namespace tech = glfw;

struct LostEngine : tech::Engine
{
    DisplayAttributes           display;
    lost::shared_ptr<Visual>   currentVisual;
    bool                        showHelp;
    Cock::Line                  mouseOrigin;

    lost::shared_ptr<Visual>   cock;
    lost::shared_ptr<Visual>   dots;
    lost::shared_ptr<Visual>   mouse;
    lost::shared_ptr<Visual>   zombong;
    lost::shared_ptr<Visual>   pfonts;
    lost::shared_ptr<Visual>   imagombies;
    lost::shared_ptr<Visual>   guirotest;

    std::vector<lost::shared_ptr<Visual> > visuals;
    unsigned long visualindex;
    lost::util::FpsMeter        fpsmeter;
    lost::guiro::ResourceLoader resourceLoader;

    void run()
    {
      showHelp = false;

      display.width       = config.get<int>("screen.width",640);
      display.height      = config.get<int>("screen.height",480);
      display.redbits     = config.get<int>("screen.redbits",8);
      display.bluebits    = config.get<int>("screen.bluebits",8);
      display.greenbits   = config.get<int>("screen.greenbits",8);
      display.alphabits   = config.get<int>("screen.alphabits",8);
      display.depthbits   = config.get<int>("screen.depthbits",8);
      display.stencilbits = config.get<int>("screen.stencilbits",0);
      display.fullscreen  = config.get<bool>("screen.fullscreen",false);

      video.restart(display); // opens window
      iout << "driver reports OpenGL version " << gl::Utils::getVersion() << lendl;

      // register callbacks
      // multiple timers are possible, but for a simple example a single timed
      // redraw callback is enough since it also gets passed the elapsed time since the last draw call
      //timer.add("gameLoop", .100);
      //timer["gameLoop"].connect(boost::bind(&LostEngine::gameLoop, this, _1));

      timer.setTimerResolution(0.001);
      timer.add("redraw", 0.015);
      timer["redraw"].connect(boost::bind(&LostEngine::redraw, this, _1));

      visuals.push_back(lost::shared_ptr<Visual>(new StupidDots(display)));
      visuals.push_back(lost::shared_ptr<Visual>(new Cock(display)));
      visuals.push_back(lost::shared_ptr<Visual>(new MouseFuxx0r(display)));
      visuals.push_back(lost::shared_ptr<Visual>(new Zombong(display)));
      visuals.push_back(lost::shared_ptr<Visual>(new Pfonts(display, resourceLoader)));
      visuals.push_back(lost::shared_ptr<Visual>(new Imagombies(display, resourceLoader)));
      visuals.push_back(lost::shared_ptr<Visual>(new Guiro(display, resourceLoader)));
      visualindex = 0;
      currentVisual = visuals[visualindex];

      // init input
      input.keyboard.connect( boost::bind( &LostEngine::keyboard, this, _1, _2 ) );
      input.mouseMove.connect( boost::bind( &LostEngine::mouseMove, this, _1 ) );
      input.mouseButton.connect( boost::bind( &LostEngine::mouseButton, this, _1, _2, _3 ) );



      // run the loop, tiers and redraws
      tech::Engine::run();
    }

    // called when a key is pressed or released
    void keyboard( int key, bool pressed )
    {
      switch (key)
      {
        case KEY_ESCAPE :
          shutdown();
          break;
        case KEY_F1 :
          showHelp = pressed;
          break;
        case KEY_F3:if(pressed){nextVisual();};break;
        case KEY_F2:if(pressed){previousVisual();};break;
        default :
          break;
      }
      currentVisual->keyboard( key, pressed );
    }

    void nextVisual()
    {
      visualindex = (visualindex + 1) % visuals.size();
      currentVisual = visuals[visualindex];
    }

    void previousVisual()
    {
      visualindex = (visualindex == 0) ? visuals.size() -1 : (visualindex - 1) % visuals.size();
      currentVisual = visuals[visualindex];
    }

    void mouseMove( lost::math::Vec2 point )
    {
      currentVisual->mouseMove( point );
    }

    void mouseButton( int button, bool pressed, lost::math::Vec2 point )
    {
      currentVisual->mouseButton( button, pressed, point );
    }

    void drawHelp()
    {
      glColor3f( 1, 0, 0 );
      lost::gl::Utils::drawString( 2,   2, "[F2] previous Visual" );
      lost::gl::Utils::drawString( 180,  2, "[F3] next Visual" );
    }

    // called periodically to update the current visual
    void redraw(double timeSinceLastCallSec)
    {
      glEnable( GL_SCISSOR_TEST );
      currentVisual->redraw(timeSinceLastCallSec);
      if (showHelp) drawHelp();
      fpsmeter.render(display.width-(fpsmeter.width+10), 0, timeSinceLastCallSec);
      video.swapBuffers();
    }
};

int main( int argc, char *argv[] )
{
  LostEngine                engine;
  lost::ConfigMap::iterator cfgIdx;

  // set the default logLevel for this application
  LogLevel( log_debug | log_info | log_warning | log_error );

  try
  {
    dout << "engine.init()" << lendl;
    engine.init( argc, argv, lost::Platform::buildResourcePath( "LostEngine.cfg" ) );

    // DEBUG: print out config variables
    for (cfgIdx = engine.config.variables.begin(); cfgIdx != engine.config.variables.end(); ++cfgIdx)
      dout << (*cfgIdx).first << " = " << (*cfgIdx).second.value << lendl;

    dout << "engine.run()" << lendl;
    engine.run();
    engine.config.saveToFile( lost::Platform::buildResourcePath( "LostEngine.cfg" ) );
  }
  catch (exception& e)
  {
    eout << "exception: " << e.what() << lendl;
  }

  return EXIT_SUCCESS;
}
