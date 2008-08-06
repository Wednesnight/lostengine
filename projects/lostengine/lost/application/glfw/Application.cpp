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

  Application::~Application() {  }

  void Application::handleResize(boost::shared_ptr<ResizeEvent> ev)
  {
    displayAttributes.width = ev->width;
    displayAttributes.height = ev->height;
  }

  void Application::run()
  {
    shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(""));
  
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
    appEvent->type = ApplicationEvent::PREINIT();dispatchEvent(appEvent);
    
    // try to extract default display attributes from lua state
    try
    {
      object g = globals(*(interpreter));
      if((luabind::type(g["config"])!= LUA_TNIL)
         && 
         (luabind::type(g["config"]["displayAttributes"])!=LUA_TNIL))
      {
        displayAttributes = object_cast<DisplayAttributes>(g["config"]["displayAttributes"]);
      }
    }
    catch(exception& ex)
    {
      IOUT("couldn't find config.displayAttributes, proceeding without, error: "<<ex.what());
    }
     
    impl->adapter.init(displayAttributes);
    // connect resize callback to ourselves so we can keep track of the window size in the displayAttributes
    addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), event::receive<ResizeEvent>(bind(&Application::handleResize, this, _1)));

    // broadcast init event so dependant code knows its safe to init resources now
    appEvent->type = ApplicationEvent::INIT();appInstance->dispatchEvent(appEvent);
        
    // broadcast postinit event to signal that the application initialisation has finished
    appEvent->type = ApplicationEvent::POSTINIT();dispatchEvent(appEvent);    
    
    appEvent->type = ApplicationEvent::RUN();dispatchEvent(appEvent);
    impl->adapter.run();
    appEvent->type = ApplicationEvent::QUIT();dispatchEvent(appEvent);
    EventDispatcher::clear();
    impl->adapter.terminate();    
  }

  void Application::quit() { impl->adapter.quit(); }
}
}
