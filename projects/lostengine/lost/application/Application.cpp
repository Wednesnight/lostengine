// FIXME: needed for NOMINMAX definition on windows
#include "lost/platform/Platform.h"

#include "lost/event/EventDispatcher.h"
#include "lost/application/Tasklet.h"
#include "lost/application/Application.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/event/Receive.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/common/Logger.h"
#include "lost/application/TaskletEvent.h"

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
        addTasklet(tasklet);
      }

      initialize();
    }
    
    Application::Application(const TaskletPtr& tasklet, resource::LoaderPtr inLoader)
    {
      if(!inLoader)
        inLoader.reset(new DefaultLoader());
      initApplication(inLoader);
      addTasklet(tasklet);
      initialize();
    }

    Application::Application(const std::string& inScript, resource::LoaderPtr inLoader)
    {
      if(!inLoader)
        inLoader.reset(new DefaultLoader());
      initApplication(inLoader);
      TaskletPtr tasklet(new Tasklet(loader));
      tasklet->script = inScript;
      addTasklet(tasklet);
      initialize();
    }

    void Application::initApplication(resource::LoaderPtr inLoader)
    {
      running = false;
      loader = inLoader;

      eventDispatcher.reset(new lost::event::EventDispatcher());
      eventDispatcher->addEventListener(ApplicationEvent::RUN(), event::receive<ApplicationEvent>(bind(&Application::startup, this, _1)));
      eventDispatcher->addEventListener(ApplicationEvent::QUIT(), event::receive<ApplicationEvent>(bind(&Application::shutdown, this, _1)));
    }

    lost::shared_ptr<Application> Application::create()
    {
      return lost::shared_ptr<Application>(new Application);
    }

    lost::shared_ptr<Application> Application::create(int argn, char** args)
    {
      return lost::shared_ptr<Application>(new Application(argn, args));
    }
    
    lost::shared_ptr<Application> Application::create(const TaskletPtr& tasklet)
    {
      return lost::shared_ptr<Application>(new Application(tasklet));
    }

    lost::shared_ptr<Application> Application::create(const std::string& inScript)
    {
      return lost::shared_ptr<Application>(new Application(inScript));
    }

    Application::~Application()
    {
    }

    void Application::startup(ApplicationEventPtr& event)
    {
      for (std::list<TaskletPtr>::iterator tasklet = tasklets.begin(); tasklet != tasklets.end(); ++tasklet)
      {
        (*tasklet)->start();
      }
      running = true;
    }
    
    void Application::addTasklet(const TaskletPtr& tasklet)
    {
      tasklet->eventDispatcher->addEventListener(ApplicationEvent::QUIT(), event::receive<ApplicationEvent>(bind(&Application::shutdown, this, _1)));
      tasklet->eventDispatcher->addEventListener(TaskletEvent::DONE(), event::receive<TaskletEvent>(bind(&Application::taskletDone, this, _1)));
      tasklets.push_back(tasklet);
      if (running)
      {
        tasklet->start();
      }
    }

    void Application::dispatchApplicationEvent(const lost::event::Type& which)
    {
      lost::shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(which));
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
      running = false;
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
      tasklets.clear();
      finalize();
    }
    


    void Application::removeTasklet(Tasklet * tasklet)
    {
      std::list<TaskletPtr>::iterator t_end = tasklets.end();
      std::list<TaskletPtr>::iterator t_iter = tasklets.begin();
      for ( ; t_iter != t_end ; ++t_iter)
      {
        if (tasklet == t_iter->get()) {
          break;
        }
      }

      if (tasklets.end() == t_iter) {
        EOUT("Oops, unknown tasklet notified us. That's not good.");
        return;
      }

      tasklets.erase(t_iter);
    }


    void Application::taskletDone(TaskletEventPtr& event)
    {
      DOUT("End of tasklet: " << event->tasklet->script);
      // FIXME: we have to do this on the mainthread, otherwise the thread blocks on its own signal
      //removeTasklet(tasklet);
      bool haveActiveTasklets = false;
      for (std::list<TaskletPtr>::iterator idx = tasklets.begin(); !running && idx != tasklets.end(); ++idx)
        haveActiveTasklets = (idx->get() != event->tasklet) && (*idx)->alive();
      if (!haveActiveTasklets)
      {
        DOUT("Last tasklet died, terminating.");
        quit();
      }
    }

  }
}
