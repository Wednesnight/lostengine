#include "lost/application/Application.h"
#include <iostream>
#include <algorithm>
#include "lost/application/Timer.h"
#include <boost/bind.hpp>
#include "lost/application/ApplicationEvent.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/lua/LuaBindings.h"
#include "lost/lua/ModuleLoader.h"

using namespace std;
using namespace boost;
using namespace lost::resource;
using namespace lost::common;
using namespace luabind;

namespace lost
{
namespace application
{
  Application* appInstance = NULL;

  /** Constructs application 
   *
   */
  Application::Application()
  : adapter(this)
  {
    // make sure the global application pointer is set to a correct value
    if(appInstance != NULL)
    {
      throw std::runtime_error("appInstance wasn't NULL, did you instance more than one app?");
    }
    appInstance = this;

    initialised = false; // init wasn't called yet
    running = false;// the main loop is not running yet
    sendIdleEvents=true; // Application sends idle events by default
    
    loader.reset(new lost::resource::DefaultLoader);// init default resource loader
    interpreter.reset(new lua::State); // init lua state with resource loader
    lost::lua::bindAll(*interpreter); // bind lostengine lua mappings    
    lost::lua::ModuleLoader::install(*interpreter, loader); // install custom module loader so require goes through resourceLoader
    luabind::globals(*interpreter)["app"] = this; // map the app itself into the interpreter so scripts can attach to its events
  }

  void Application::init()
  {
    if(initialised) return;
    initialised = true;
    
    // try to load default lua script
    try
    {
      shared_ptr<File> initScript = loader->load("init");
      interpreter->doString(initScript->str());
    }
    catch(exception& ex)
    {
      IOUT("couldn't load init script, proceeeding without it, error: "+string(ex.what()));
    }
    
    // broadcast preinit event, this is the latest point for client code to setup the configuration
    shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(ApplicationEvent::PREINIT()));dispatchEvent(appEvent);
    
    // try to extract default display attributes from lua state
    try
    {
      displayAttributes = object_cast<DisplayAttributes>(globals(*interpreter)["config"]["displayAttributes"]);
    }
    catch(exception& ex)
    {
      IOUT("couldn't find config.displayAttributes, proceeding without, error: "<<ex.what());
    }
    // init display/adapter so all other code is safe from here on because an OpenGL context now exists
    adapter.init(displayAttributes);
    
    // broadcast init event so dependant code knows its safe to init resources now
    appEvent->type = ApplicationEvent::INIT();dispatchEvent(appEvent);
    
    // connect resize callback to ourselves so we can keep track of the window size in the displayAttributes
    addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), event::receive<ResizeEvent>(bind(&Application::handleResize, this, _1)));
    
    // broadcast postinit event to signal that the application initialisation has finished
    appEvent->type = ApplicationEvent::POSTINIT();dispatchEvent(appEvent);    
  }
  
  Application::~Application() { adapter.terminate(); }

  void Application::handleResize(boost::shared_ptr<ResizeEvent> ev)
  {
    displayAttributes.width = ev->width;
    displayAttributes.height = ev->height;
  }

  void Application::run()
  {
    init();
    shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(""));
    appEvent->type = ApplicationEvent::RUN();dispatchEvent(appEvent);
    running = true;
    double lastTime = adapter.getTime();
    while(running)
    {
      double now = adapter.getTime();
      double delta = now - lastTime;
      lastTime = now;
      timerManager.updateTimers(delta);
      adapter.pollEvents();
      if(sendIdleEvents)
      {
        appEvent->type = ApplicationEvent::IDLE();dispatchEvent(appEvent);
      }
      adapter.sleep(.001); // stupid fucking hack because glfw doesn't support OS timers and we need to measure manually.
      if(!adapter.displayOpen())
      {
        running = false;
      }
    }
    appEvent->type = ApplicationEvent::QUIT();dispatchEvent(appEvent);
    EventDispatcher::clear();
  }

  void Application::quit() { running=false; }
}
}
