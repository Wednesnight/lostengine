#include "lost/application/Application.h"
#include <iostream>
#include <algorithm>
#include "lost/application/Timer.h"
#include <boost/bind.hpp>
#include "lost/application/ApplicationEvent.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/lua/LuaBindings.h"
#include "lost/lua/ModuleLoader.h"
#include "lost/application/glfw/LostApplicationImpl.h"

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
  {
    // make sure the global application pointer is set to a correct value
    if(appInstance != NULL)
    {
      throw std::runtime_error("appInstance wasn't NULL, did you instance more than one app?");
    }
    appInstance = this;
    impl.reset(new LostApplicationImpl(this));
    
    
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
