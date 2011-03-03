#include <boost/bind.hpp>
#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/Tasklet.h"
#include "lost/application/Application.h"
#include "lost/event/Receive.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/common/Logger.h"
#include "lost/application/TaskletEvent.h"
#include "lost/application/SpawnTaskletEvent.h"
#include "lost/application/QueueEvent.h"
#include "lost/application/ProcessEvent.h"
#include "lost/application/Window.h"

using namespace lost::resource;

namespace lost
{
  namespace application
  {

    ApplicationPtr Application::getInstance(Tasklet* tasklet)
    {
      static ApplicationPtr instance;
      if (!instance) {
        instance.reset(new Application(tasklet));
      }
      return instance;
    }

    Application::Application(Tasklet* tasklet)
    : running(false)
    {
      eventDispatcher.reset(new lost::event::EventDispatcher());
      eventDispatcher->addEventListener(ApplicationEvent::RUN(), event::receive<ApplicationEvent>(bind(&Application::startup, this, _1)));
      eventDispatcher->addEventListener(ApplicationEvent::QUIT(), event::receive<ApplicationEvent>(bind(&Application::quitHandler, this, _1)));
      eventDispatcher->addEventListener(SpawnTaskletEvent::SPAWN_TASKLET(), event::receive<SpawnTaskletEvent>(bind(&Application::taskletSpawn, this, _1)));
      eventDispatcher->addEventListener(TaskletEvent::TERMINATE(), event::receive<TaskletEvent>(bind(&Application::taskletTerminate, this, _1)));
      eventDispatcher->addEventListener(TaskletEvent::DONE(), event::receive<TaskletEvent>(bind(&Application::taskletDone, this, _1)));

      if (tasklet != NULL) {
        addTasklet(tasklet);
      }
      initialize();
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
        try
        {
          (*tasklet)->start();
          eventDispatcher->dispatchEvent(TaskletEvent::create(TaskletEvent::START(), *tasklet));
        }
        catch(std::exception& ex)
        {
          EOUT("Tasklet failed with error: "<<ex.what())
            // just to make sure that we're running on the main thread
          queueEvent(QueueEventPtr(new QueueEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), *tasklet)))));
        }
      }
      running = true;
      processEvents(ProcessEventPtr());
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
          tasklet->start();
          eventDispatcher->dispatchEvent(TaskletEvent::create(TaskletEvent::START(), tasklet));
        }
        catch(std::exception& ex)
        {
          EOUT("Tasklet failed with error: "<<ex.what())
          // just to make sure that we're running on the main thread
          queueEvent(QueueEventPtr(new QueueEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), tasklet)))));
          processEvents(ProcessEventPtr());
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
          (*tasklet)->eventDispatcher->queueEvent(event);
          (*tasklet)->stop();
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

      (*t_iter)->stop();
      DOUT("End of tasklet: " << (*t_iter)->name);
      delete *t_iter;
      tasklets.erase(t_iter);
    }

    void Application::taskletTerminate(const TaskletEventPtr& event)
    {
      event->tasklet->stop();
    }

    void Application::taskletDone(const TaskletEventPtr& event)
    {
      Tasklet* currentTasklet = event->tasklet;
      removeTasklet(currentTasklet);
      bool haveActiveTasklets = false;
      for (std::list<Tasklet*>::iterator idx = tasklets.begin(); idx != tasklets.end(); ++idx)
      {
        haveActiveTasklets = (*idx)->alive();
        if (haveActiveTasklets) break;
      }

      // if we're still running and there's no tasklet left we initialize a shutdown
      if (running && !haveActiveTasklets)
      {
        DOUT("Last tasklet died, terminating.");
        quit();
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
