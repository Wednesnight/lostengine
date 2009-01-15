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
#include "lost/event/Receive.h"

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
    
    void Application::init()
    {
      // make sure the global application pointer is set to a correct value
      if(appInstance != NULL)
      {
        throw std::runtime_error("appInstance wasn't NULL, did you instance more than one app?");
      }
      appInstance = this;
      adapter.reset(new ApplicationAdapter(this));
      displayAttributes.reset(new DisplayAttributes());

      loader.reset(new lost::resource::DefaultLoader);// init default resource loader
      interpreter.reset(new lua::State(true, true, true, loader)); // init lua state with resource loader
      lost::lua::bindAll(*interpreter); // bind lostengine lua mappings    

      globals(*interpreter)["lost"]["globals"]                   = newtable(*interpreter); // create globals table
      globals(*interpreter)["lost"]["globals"]["app"]            = this; // map the app itself into the interpreter so scripts can attach to its events
      luabind::globals(*interpreter)["lost"]["globals"]["state"] = interpreter; // map the state itself into the interpreter so scripts can use it

      lost::lua::ModuleLoader::install(*interpreter, loader); // install custom module loader so require goes through resourceLoader

      config.reset(new Config(interpreter)); // init config
    }

    Application::Application()
    {
      init();
    }

    Application::Application(boost::function<void (void)> mainLoopFunc)
    {
      mainLoop = boost::shared_ptr<MainLoop>(new FunctorMainLoop(mainLoopFunc));
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

    void Application::run()
    {
      shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(""));
    
      // try to load default lua script
      try
      {
        shared_ptr<File> initScript = loader->load("init");
        interpreter->doFile(initScript);
      }
      catch(std::exception& ex)
      {
        IOUT("couldn't load init script, proceeding without it, error: "+string(ex.what()));
      }
      
      // broadcast preinit event, this is the latest point for c++ client code to setup the configuration
      appEvent->type = ApplicationEvent::PREINIT();dispatchEvent(appEvent);
      
      // try to extract default display attributes from lua state
      displayAttributes = config["displayAttributes"].as<shared_ptr<DisplayAttributes> >(displayAttributes);

      // connect resize callback to ourselves so we can keep track of the window size in the displayAttributes
      addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), event::receive<ResizeEvent>(bind(&Application::handleResize, this, _1)));

      // the gl context will be created here, so resize event needs to be connected before
      adapter->init(displayAttributes);
      // init gl context
      context.reset(new Context(displayAttributes));

      // broadcast init event so dependant code knows its safe to init resources now
      appEvent->type = ApplicationEvent::INIT();appInstance->dispatchEvent(appEvent);
              
      appEvent->type = ApplicationEvent::RUN();dispatchEvent(appEvent);
      adapter->mainLoop = mainLoop;
      boost::function<void (boost::shared_ptr<event::Event>)> f = boost::bind(&MainLoop::quitEventHandler, mainLoop.get(), _1);
      addEventListener(ApplicationEvent::QUIT(), f);
      adapter->run();
      appEvent->type = ApplicationEvent::QUIT(); adapter->queueEvent(appEvent);
      if(adapter->mainLoopThread)
      {
        adapter->mainLoopThread->join();
      }
      EventDispatcher::clear();
      adapter->terminate();    
    }

    void Application::processEvents(const double& timeoutInSeconds)
    {
      adapter->processEvents();
    }
    
    void Application::swapBuffers()
    {
      adapter->swapBuffers();
    }

    void Application::quit()
    {
      adapter->quit();
    }

  }
}
