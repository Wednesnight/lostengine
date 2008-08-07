#include "lost/application/iphone/LostApplicationHelpers.h"
#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include <boost/bind.hpp>
using namespace std;
using namespace boost;
using namespace lost;
using namespace lost::resource;
using namespace lost::common;
using namespace lost::application;
using namespace luabind;

void lostApplicationHelpers_preinit()
{
  shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(""));
  // connect resize callback to ourselves so we can keep track of the window size in the displayAttributes
  appInstance->addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), event::receive<ResizeEvent>(bind(&Application::handleResize, appInstance, _1)));
  
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

  // broadcast preinit event, this is the latest point for c++ client code to setup the configuration
  appEvent->type = ApplicationEvent::PREINIT();appInstance->dispatchEvent(appEvent);
}

void lostApplicationHelpers_init()
{
  shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(""));

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
  
  
  // connect resize callback to ourselves so we can keep track of the window size in the displayAttributes
  appInstance->addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), event::receive<ResizeEvent>(bind(&Application::handleResize, appInstance, _1)));
  
  // no adapter init called since its all done in the uikit app controller
  
  // broadcast init event so dependant code knows its safe to init resources now
  appEvent->type = ApplicationEvent::INIT();appInstance->dispatchEvent(appEvent);  
}

void lostApplicationHelpers_run()
{
  shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(""));
  appEvent->type = ApplicationEvent::RUN();appInstance->dispatchEvent(appEvent);  
}

void lostApplicationHelpers_quit()
{
  shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(""));
  appEvent->type = ApplicationEvent::QUIT();appInstance->dispatchEvent(appEvent);
  appInstance->clear();
}

