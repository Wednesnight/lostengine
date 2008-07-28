#include "lost/application/Application.h"
#include <iostream>
#include <algorithm>
#include "lost/application/Timer.h"
#include <boost/bind.hpp>
#include "lost/application/ApplicationEvent.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/lua/LuaBindings.h"
#include "lost/lua/ModuleLoader.h"
#include "lost/application/glfw/GlfwAdapter.h"
#include "lost/application/glfw/TimerManager.h"

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

  struct ApplicationImpl
  {
    GlfwAdapter   adapter;
    TimerManager  timerManager;
    bool          initialised;
    bool          running;

    ApplicationImpl()
    : adapter(appInstance) // instantiate impl AFTER appInstance pointer was set!
    {
      initialised = false;
      running = false;// the main loop is not running yet
    }
    
    
    void handleResize(boost::shared_ptr<ResizeEvent> ev)
    {
      appInstance->displayAttributes.width = ev->width;
      appInstance->displayAttributes.height = ev->height;
    }
        
    void init()
    {
      if(initialised) return;
      initialised = true;
      
      // try to load default lua script
      try
      {
        shared_ptr<File> initScript = appInstance->loader->load("init");
        appInstance->interpreter->doString(initScript->str());
      }
      catch(exception& ex)
      {
        IOUT("couldn't load init script, proceeeding without it, error: "+string(ex.what()));
      }
      
      // broadcast preinit event, this is the latest point for client code to setup the configuration
      shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(ApplicationEvent::PREINIT()));appInstance->dispatchEvent(appEvent);
      
      // try to extract default display attributes from lua state
      try
      {
        object g = globals(*(appInstance->interpreter));
        if((luabind::type(g["config"])!= LUA_TNIL)
           && 
           (luabind::type(g["config"]["displayAttributes"])!=LUA_TNIL))
        {
          appInstance->displayAttributes = object_cast<DisplayAttributes>(g["config"]["displayAttributes"]);
        }
      }
      catch(exception& ex)
      {
        IOUT("couldn't find config.displayAttributes, proceeding without, error: "<<ex.what());
      }
      // init display/adapter so all other code is safe from here on because an OpenGL context now exists
      adapter.init(appInstance->displayAttributes);
      
      // broadcast init event so dependant code knows its safe to init resources now
      appEvent->type = ApplicationEvent::INIT();appInstance->dispatchEvent(appEvent);
      
      // connect resize callback to ourselves so we can keep track of the window size in the displayAttributes
      appInstance->addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), event::receive<ResizeEvent>(bind(&ApplicationImpl::handleResize, this, _1)));
      
      // broadcast postinit event to signal that the application initialisation has finished
      appEvent->type = ApplicationEvent::POSTINIT();appInstance->dispatchEvent(appEvent);    
    }
  };
  
  /** Constructs application 
   *
   */
  Application::Application()
  {
    // make sure the global application pointer is set to a correct value
    if(appInstance != NULL)
    {
      throw std::runtime_error("appInstance wasn't NULL, did you instance more than one app?");
    }
    appInstance = this;
    impl.reset(new ApplicationImpl);
    
    
    loader.reset(new lost::resource::DefaultLoader);// init default resource loader
    interpreter.reset(new lua::State); // init lua state with resource loader
    lost::lua::bindAll(*interpreter); // bind lostengine lua mappings    
    lost::lua::ModuleLoader::install(*interpreter, loader); // install custom module loader so require goes through resourceLoader
    luabind::globals(*interpreter)["app"] = this; // map the app itself into the interpreter so scripts can attach to its events
  }

  Application::~Application() { impl->adapter.terminate(); }

  void Application::run()
  {
    impl->init();
    shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(""));
    appEvent->type = ApplicationEvent::RUN();dispatchEvent(appEvent);
    impl->running = true;
    double lastTime = impl->adapter.getTime();
    while(impl->running)
    {
      double now = impl->adapter.getTime();
      double delta = now - lastTime;
      lastTime = now;
      impl->timerManager.updateTimers(delta);
      impl->adapter.pollEvents();
      impl->adapter.sleep(.001); // don't eat CPU time with a pure spin wait, sleep in intervals
      if(!impl->adapter.displayOpen())
      {
        impl->running = false;
      }
    }
    appEvent->type = ApplicationEvent::QUIT();dispatchEvent(appEvent);
    EventDispatcher::clear();
  }

  void Application::quit() { impl->running=false; }
}
}
