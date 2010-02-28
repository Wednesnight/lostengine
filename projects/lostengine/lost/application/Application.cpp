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
#include "lost/application/SpawnTaskletEvent.h"
#include "lost/application/QueueEvent.h"
#include "lost/application/ProcessEvent.h"
#include "lost/gl/Context.h"
#include "lost/application/Window.h"

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
    
    Application::Application(Tasklet* tasklet, resource::LoaderPtr inLoader)
    {
      if(!inLoader)
        inLoader.reset(new DefaultLoader());
      initApplication(inLoader);
      addTasklet(tasklet);
      initialize();
    }

    void Application::initApplication(resource::LoaderPtr inLoader)
    {
      running = false;
      loader = inLoader;

      eventDispatcher.reset(new lost::event::EventDispatcher());
      eventDispatcher->addEventListener(ApplicationEvent::RUN(), event::receive<ApplicationEvent>(bind(&Application::startup, this, _1)));
      eventDispatcher->addEventListener(ApplicationEvent::QUIT(), event::receive<ApplicationEvent>(bind(&Application::quitHandler, this, _1)));
      eventDispatcher->addEventListener(SpawnTaskletEvent::SPAWN_TASKLET(), event::receive<SpawnTaskletEvent>(bind(&Application::taskletSpawn, this, _1)));
      eventDispatcher->addEventListener(TaskletEvent::DONE(), event::receive<TaskletEvent>(bind(&Application::taskletDone, this, _1)));
    }

    lost::shared_ptr<Application> Application::create()
    {
      return lost::shared_ptr<Application>(new Application);
    }
    
    lost::shared_ptr<Application> Application::create(Tasklet* tasklet)
    {
      return lost::shared_ptr<Application>(new Application(tasklet));
    }

    Application::~Application()
    {
      for (std::list<Tasklet*>::iterator tasklet = tasklets.begin(); tasklet != tasklets.end(); ++tasklet)
      {
        delete *tasklet;
      }
      tasklets.clear();
    }

    void Application::startup(ApplicationEventPtr& event)
    {
      for (std::list<Tasklet*>::iterator tasklet = tasklets.begin(); tasklet != tasklets.end(); ++tasklet)
      {
        (*tasklet)->init();
        (*tasklet)->start();
      }
      running = true;
    }
    
    void Application::addTasklet(Tasklet* tasklet)
    {
      tasklet->eventDispatcher->addEventListener(QueueEvent::QUEUE(), event::receive<QueueEvent>(bind(&Application::queueEvent, this, _1)));
      tasklet->eventDispatcher->addEventListener(ProcessEvent::PROCESS(), event::receive<ProcessEvent>(bind(&Application::processEvents, this, _1)));
      tasklets.push_back(tasklet);
      if (running)
      {
        try
        {
          tasklet->init();
          tasklet->start();
        }
        catch(std::exception& ex)
        {
          EOUT("Tasklet failed with error: "<<ex.what())
        }
      }
    }

    void Application::quitHandler(ApplicationEventPtr& event)
    {
      running = false;
      for (std::list<Tasklet*>::iterator tasklet = tasklets.begin(); tasklet != tasklets.end(); ++tasklet)
      {
        if ((*tasklet)->alive())
        {
          (*tasklet)->stop();
          (*tasklet)->eventDispatcher->queueEvent(event);
          (*tasklet)->wait();
        }
      }
      shutdown();
    }

    void Application::removeTasklet(Tasklet * tasklet)
    {
      std::list<Tasklet*>::iterator t_end = tasklets.end();
      std::list<Tasklet*>::iterator t_iter = tasklets.begin();
      for ( ; t_iter != t_end ; ++t_iter)
      {
        if (tasklet == *t_iter) {
          break;
        }
      }

      if (tasklets.end() == t_iter) {
        EOUT("Oops, unknown tasklet notified us. That's not good.");
        return;
      }

      Tasklet* t = *t_iter;
      void* glctx = NULL;
      if(t->window)
      {
        glctx = gl::Context::getCurrentOsSpecific();
        t->window->context->makeCurrent();
      }
      delete *t_iter;
      if(glctx)
        gl::Context::setCurrentOsSpecififc(glctx);
      tasklets.erase(t_iter);
    }

    void Application::taskletDone(const TaskletEventPtr& event)
    {
      Tasklet* currentTasklet = reinterpret_cast<Tasklet*>(event->tasklet);
      DOUT("End of tasklet: " << currentTasklet->name);
      removeTasklet(currentTasklet);
      bool haveActiveTasklets = false;
      for (std::list<Tasklet*>::iterator idx = tasklets.begin(); idx != tasklets.end(); ++idx)
      {
        haveActiveTasklets = (*idx)->alive();
        if (haveActiveTasklets) break;
      }
      if (!haveActiveTasklets)
      {
        DOUT("Last tasklet died, terminating.");
        shutdown();
      }
    }

    void Application::queueEvent(const QueueEventPtr& event)
    {
      eventDispatcher->queueEvent(event->event);
    }

    void Application::quit()
    {
      // just to make sure that we're running on the main thread
      queueEvent(QueueEventPtr(new QueueEvent(ApplicationEventPtr(new ApplicationEvent(ApplicationEvent::QUIT())))));
      processEvents(ProcessEventPtr());
    }

  }
}
