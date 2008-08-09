#include "lost/application/Application.h"
#include <iostream>
#include <algorithm>
#include "lost/application/Timer.h"
#include <boost/bind.hpp>
#include "lost/application/ApplicationEvent.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/lua/LuaBindings.h"
#include "lost/lua/ModuleLoader.h"

#include "lost/application/iphone/LostApplicationHelpers.h"

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
  void* appControllerLinkHelper = NULL;
    
  /** Constructs application 
   *
   */
  Application::Application()
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

  void Application::run() { lostApplicationHelpers_runAppController(); }
  void Application::swapBuffers() { lostApplicationHelpers_swapBuffers(); }
  void Application::quit() { lostApplicationHelpers_quitAppController(); }
}
}
