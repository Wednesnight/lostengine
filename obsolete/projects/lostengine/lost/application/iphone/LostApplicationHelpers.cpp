#include "lost/application/iphone/LostApplicationHelpers.h"
#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include <boost/bind.hpp>
#include "lost/event/Receive.h"

using namespace std;
using namespace boost;
using namespace lost;
using namespace lost::resource;
using namespace lost::common;
using namespace lost::application;
using namespace lost::gl;
using namespace luabind;

boost::shared_ptr<boost::thread> adapterMainLoopThread;

void lostApplicationHelpers_dispatchEvent(boost::shared_ptr<lost::event::Event> event)
{
  appInstance->queueEvent(event);
}

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
  catch(std::exception& ex)
  {
    IOUT("couldn't load init script, proceeeding without it, error: "+string(ex.what()));
  }    

  // broadcast preinit event, this is the latest point for c++ client code to setup the configuration
  appEvent->type = ApplicationEvent::PREINIT();appInstance->dispatchEvent(appEvent);
}

void lostApplicationHelpers_resize(unsigned short width, unsigned short height)
{
  shared_ptr<ResizeEvent> event(new ResizeEvent(ResizeEvent::MAIN_WINDOW_RESIZE()));
  event->width = width;
  event->height = height;
  appInstance->queueEvent(event);
}

void lostApplicationHelpers_init()
{
  shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(""));

  // try to extract default display attributes from lua state
  appInstance->displayAttributes = appInstance->config["displayAttributes"].as<shared_ptr<DisplayAttributes> >(appInstance->displayAttributes);
  
  // no adapter init called since its all done in the uikit app controller

  // init gl context
  appInstance->context.reset(new Context(appInstance->displayAttributes));

  // broadcast init event so dependant code knows its safe to init resources now
  appEvent->type = ApplicationEvent::INIT();appInstance->dispatchEvent(appEvent);  
}

void lostApplicationHelpers_run()
{
  shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(""));
  appEvent->type = ApplicationEvent::RUN();appInstance->dispatchEvent(appEvent);  
  boost::function<void (boost::shared_ptr<event::Event>)> f = boost::bind(&MainLoop::quitEventHandler, appInstance->mainLoop.get(), _1);
  appInstance->addEventListener(ApplicationEvent::QUIT(), f);
  // start the main loop
  if(appInstance->mainLoop)
  {
    adapterMainLoopThread.reset(new boost::thread(boost::bind(&lost::application::MainLoop::run, appInstance->mainLoop.get())));
  }
}

void lostApplicationHelpers_quit()
{
  shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(""));
  appEvent->type = ApplicationEvent::QUIT();appInstance->queueEvent(appEvent);
  if(adapterMainLoopThread)
  {
    adapterMainLoopThread->join();
  }
  appInstance->clear();
}