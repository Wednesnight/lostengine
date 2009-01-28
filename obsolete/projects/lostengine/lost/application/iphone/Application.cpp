#include "lost/application/Application.h"
#include <iostream>
#include <algorithm>
#include "lost/application/Timer.h"
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include "lost/application/ApplicationEvent.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/lua/BindAll.h"
#include "lost/lua/ModuleLoader.h"
#include "lost/event/Receive.h"
#include "lost/application/iphone/LostApplicationHelpers.h"

using namespace std;
using namespace boost;
using namespace lost::resource;
using namespace lost::common;
using namespace lost::gl;
using namespace luabind;

namespace lost
{
  namespace application
  {
    Application* appInstance = NULL;
    void* appControllerLinkHelper = NULL;

    void Application::init()
    {
      // curiously, if we don't inszantiate the controller once from here, the code will be stripped from the lib,
      // and any application will fail with "no class named LostAppController is loaded"
      // this is why the linkHelper instantiates the controller once and returns an instance as void*
      appControllerLinkHelper = lostApplicationHelpers_linkHelper();
      DOUT("after link helper");
      // make sure the global application pointer is set to a correct value
      if(appInstance != NULL)
      {
        throw std::runtime_error("appInstance wasn't NULL, did you instance more than one app?");
      }
      appInstance = this;
      
      displayAttributes.reset(new DisplayAttributes());

      loader.reset(new lost::resource::DefaultLoader);// init default resource loader
      interpreter.reset(new lua::State(true, true, true, loader)); // init lua state with resource loader
      lost::lua::bindAll(*interpreter); // bind lostengine lua mappings    

      globals(*interpreter)["lost"]["globals"]                   = newtable(*interpreter); // create globals table
      globals(*interpreter)["lost"]["globals"]["app"]            = this; // map the app itself into the interpreter so scripts can attach to its events
      luabind::globals(*interpreter)["lost"]["globals"]["state"] = interpreter; // map the state itself into the interpreter so scripts can use it

      lost::lua::ModuleLoader::install(*interpreter, loader); // install custom module loader so require goes through resourceLoader

      config.reset(new Config(interpreter)); // init config

      // connect resize callback to ourselves so we can keep track of the window size in the displayAttributes
      appInstance->addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), event::receive<ResizeEvent>(bind(&Application::handleResize, appInstance, _1)));    
    }

    Application::Application()
    {
      init();
    }
    
    Application::Application(boost::function<void (void)> mainLoopFunc)
    {
      mainLoop = boost::shared_ptr<MainLoop>(new FunctorMainLoop(context, mainLoopFunc));
      init();
    }
    
    Application::Application(boost::shared_ptr<MainLoop> inMainLoop)
    {
      mainLoop = inMainLoop;
      init();
    }
    
    Application::~Application() {  }

    void Application::handleResize(boost::shared_ptr<ResizeEvent> ev)
    {
      displayAttributes->width = ev->width;
      displayAttributes->height = ev->height;
    }

    void Application::run() { lostApplicationHelpers_runAppController(); }
    void Application::swapBuffers() { lostApplicationHelpers_swapBuffers(); }
    void Application::quit() { lostApplicationHelpers_quitAppController(); }
  }
}
