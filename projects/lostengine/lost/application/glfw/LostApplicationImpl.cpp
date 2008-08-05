#include "lost/application/glfw/LostApplicationImpl.h"
#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/common/DisplayAttributes.h"

using namespace lost::event;
using namespace lost;
using namespace lost::application;
using namespace boost;
using namespace lost::resource;
using namespace lost::common;
using namespace std;
using namespace luabind;

LostApplicationImpl::LostApplicationImpl(EventDispatcher* target)
: adapter(target), delegate(target) // instantiate impl AFTER appInstance pointer was set!
{
  initialised = false;
  running = false;// the main loop is not running yet
}


void LostApplicationImpl::handleResize(boost::shared_ptr<ResizeEvent> ev)
{
  appInstance->displayAttributes.width = ev->width;
  appInstance->displayAttributes.height = ev->height;
}
    
void LostApplicationImpl::init()
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
  appInstance->addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), event::receive<ResizeEvent>(bind(&LostApplicationImpl::handleResize, this, _1)));
  
  // broadcast postinit event to signal that the application initialisation has finished
  appEvent->type = ApplicationEvent::POSTINIT();appInstance->dispatchEvent(appEvent);    
}
