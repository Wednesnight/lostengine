#include "lost/application/Application.h"
#include "lost/application/glfw/ApplicationAdapter.h"
#include <iostream>
#include <algorithm>
#include "lost/application/Timer.h"
#include <boost/bind.hpp>
#include "lost/application/ApplicationEvent.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/lua/BindAll.h"
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
  {
    // make sure the global application pointer is set to a correct value
    if(appInstance != NULL)
    {
      throw std::runtime_error("appInstance wasn't NULL, did you instance more than one app?");
    }
    appInstance = this;
    adapter.reset(new ApplicationAdapter(this));
    
    loader.reset(new lost::resource::DefaultLoader);// init default resource loader
    interpreter.reset(new lua::State); // init lua state with resource loader
    lost::lua::bindAll(*interpreter); // bind lostengine lua mappings    

    globals(*interpreter)["globals"]                   = newtable(*interpreter); // create globals table
    globals(*interpreter)["globals"]["app"]            = this; // map the app itself into the interpreter so scripts can attach to its events
    luabind::globals(*interpreter)["globals"]["state"] = interpreter; // map the state itself into the interpreter so scripts can use it
    
    lost::lua::ModuleLoader::install(*interpreter, loader); // install custom module loader so require goes through resourceLoader

    config.reset(new Config(interpreter)); // init config
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
      interpreter->doFile(initScript);
    }
    catch(exception& ex)
    {
      IOUT("couldn't load init script, proceeding without it, error: "+string(ex.what()));
    }
    
    // broadcast preinit event, this is the latest point for c++ client code to setup the configuration
    appEvent->type = ApplicationEvent::PREINIT();dispatchEvent(appEvent);
    
    // try to extract default display attributes from lua state
    displayAttributes = config["displayAttributes"].as<DisplayAttributes>(displayAttributes);

    // connect resize callback to ourselves so we can keep track of the window size in the displayAttributes
    addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), event::receive<ResizeEvent>(bind(&Application::handleResize, this, _1)));

    // the gl context will be created here, so resize event needs to be connected before
    adapter->init(displayAttributes);

    // broadcast init event so dependant code knows its safe to init resources now
    appEvent->type = ApplicationEvent::INIT();appInstance->dispatchEvent(appEvent);
            
    appEvent->type = ApplicationEvent::RUN();dispatchEvent(appEvent);
    adapter->run();
    appEvent->type = ApplicationEvent::QUIT();dispatchEvent(appEvent);
    EventDispatcher::clear();
    adapter->terminate();    
  }

  void Application::swapBuffers() {adapter->swapBuffers();}

  void Application::quit() { adapter->quit(); }
}
}
