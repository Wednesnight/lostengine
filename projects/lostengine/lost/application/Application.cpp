// FIXME: needed for NOMINMAX definition on windows
#include "lost/platform/Platform.h"

#include "lost/event/EventDispatcher.h"
#include "lost/application/Tasklet.h"
#include "lost/application/Application.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/event/Receive.h"
#include "lost/application/ApplicationEvent.h"

#include <boost/program_options.hpp>

using namespace lost::resource;

namespace lost
{
  namespace application
  {
    
    Application::Application(resource::LoaderPtr inLoader)
    {
      if(!inLoader)
        inLoader.reset(new DefaultLoader());
      initApplication(inLoader);
      initialize();
    }

    Application::Application(int argn, char** args, resource::LoaderPtr inLoader)
    {
      if(!inLoader)
        inLoader.reset(new DefaultLoader());
      initApplication(inLoader);

      using namespace boost::program_options;
      
      // Declare the supported options.
      options_description desc;
      desc.add_options()
      ("script", value<std::string>())
      ;
      positional_options_description p;
      p.add("script", -1);
      
      variables_map vm;
      store(command_line_parser(argn, args).options(desc).positional(p).run(), vm);
      notify(vm);
      
      if (vm.count("script"))
      {
        TaskletPtr tasklet(new Tasklet(loader));
        tasklet->script = vm["script"].as<std::string>();
        tasklets.push_back(tasklet);
      }

      initialize();
    }
    
    Application::Application(const TaskletPtr& tasklet, resource::LoaderPtr inLoader)
    {
      if(!inLoader)
        inLoader.reset(new DefaultLoader());
      initApplication(inLoader);
      tasklets.push_back(tasklet);
      initialize();
    }

    Application::Application(const std::string& inScript, resource::LoaderPtr inLoader)
    {
      if(!inLoader)
        inLoader.reset(new DefaultLoader());
      initApplication(inLoader);
      TaskletPtr tasklet(new Tasklet(loader));
      tasklet->script = inScript;
      tasklets.push_back(tasklet);
      initialize();
    }

    void Application::initApplication(resource::LoaderPtr inLoader)
    {
      loader = inLoader;

      eventDispatcher.reset(new lost::event::EventDispatcher());
      eventDispatcher->addEventListener(ApplicationEvent::RUN(), event::receive<ApplicationEvent>(bind(&Application::startup, this, _1)));
      eventDispatcher->addEventListener(ApplicationEvent::QUIT(), event::receive<ApplicationEvent>(bind(&Application::shutdown, this, _1)));
    }

    void Application::startup(ApplicationEventPtr& event)
    {
      for (std::list<TaskletPtr>::iterator tasklet = tasklets.begin(); tasklet != tasklets.end(); ++tasklet)
      {
        (*tasklet)->eventDispatcher->addEventListener(ApplicationEvent::QUIT(), event::receive<ApplicationEvent>(bind(&Application::shutdown, this, _1)));
        (*tasklet)->start();
      }
    }
    
    boost::shared_ptr<Application> Application::create()
    {
      return boost::shared_ptr<Application>(new Application);
    }

    boost::shared_ptr<Application> Application::create(int argn, char** args)
    {
      return boost::shared_ptr<Application>(new Application(argn, args));
    }
    
    boost::shared_ptr<Application> Application::create(const TaskletPtr& tasklet)
    {
      return boost::shared_ptr<Application>(new Application(tasklet));
    }

    boost::shared_ptr<Application> Application::create(const std::string& inScript)
    {
      return boost::shared_ptr<Application>(new Application(inScript));
    }

    Application::~Application()
    {
      finalize();
    }

    void Application::addTasklet(const TaskletPtr& tasklet)
    {
      tasklets.push_back(tasklet);
    }

    void Application::dispatchApplicationEvent(const lost::event::Type& which)
    {
      boost::shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(which));
      eventDispatcher->dispatchEvent(appEvent);
      for (std::list<TaskletPtr>::iterator tasklet = tasklets.begin(); tasklet != tasklets.end(); ++tasklet)
      {
        if ((*tasklet)->alive())
        {
          (*tasklet)->eventDispatcher->queueEvent(appEvent);
        }
      }
    }

    void Application::quit()
    {
      dispatchApplicationEvent(ApplicationEvent::QUIT());
    }

    void Application::terminate()
    {
      for (std::list<TaskletPtr>::iterator tasklet = tasklets.begin(); tasklet != tasklets.end(); ++tasklet)
      {
        if ((*tasklet)->alive())
        {
          (*tasklet)->stop();
          (*tasklet)->wait();
        }
      }
    }
    
  }
}
