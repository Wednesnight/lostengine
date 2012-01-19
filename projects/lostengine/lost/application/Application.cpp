/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/Tasklet.h"
#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/common/Logger.h"
#include "lost/application/TaskletEvent.h"
#include "lost/application/SpawnTaskletEvent.h"
#include "lost/application/QueueEvent.h"
#include "lost/application/ProcessEvent.h"
#include "lost/application/Window.h"
#include "lost/event/Listener.h"

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

      eventDispatcher->addEventListener(ApplicationEvent::RUN(), event::makeListener(this, &Application::startup));
      eventDispatcher->addEventListener(ApplicationEvent::QUIT(), event::makeListener(this, &Application::quitHandler));
      eventDispatcher->addEventListener(SpawnTaskletEvent::SPAWN_TASKLET(), event::makeListener(this, &Application::taskletSpawn));
      eventDispatcher->addEventListener(TaskletEvent::TERMINATE(), event::makeListener(this, &Application::taskletTerminate));
      eventDispatcher->addEventListener(TaskletEvent::DONE(), event::makeListener(this, &Application::taskletDone));

      if (tasklet != NULL) {
        addTasklet(tasklet);
      }
      platform::setThreadName("LostEngine main thread");
      initialize();
    }

    Application::~Application()
    {
      for (list<Tasklet*>::iterator tasklet = tasklets.begin(); tasklet != tasklets.end(); ++tasklet)
      {
        delete *tasklet;
      }
      tasklets.clear();
    }

    void Application::startup(const ApplicationEventPtr& event)
    {
      for (list<Tasklet*>::iterator tasklet = tasklets.begin(); tasklet != tasklets.end(); ++tasklet)
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
      tasklet->eventDispatcher->addEventListener(QueueEvent::QUEUE(), event::makeListener(this, &Application::queueEvent));
      tasklet->eventDispatcher->addEventListener(ProcessEvent::PROCESS(), event::makeListener(this, &Application::processEvents));
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

    void Application::quitHandler(const ApplicationEventPtr& event)
    {
      running = false;
      for (list<Tasklet*>::iterator tasklet = tasklets.begin(); tasklet != tasklets.end(); ++tasklet)
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
      list<Tasklet*>::iterator t_end = tasklets.end();
      list<Tasklet*>::iterator t_iter = tasklets.begin();
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
      for (list<Tasklet*>::iterator idx = tasklets.begin(); idx != tasklets.end(); ++idx)
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
      ApplicationEventPtr ae(new ApplicationEvent(ApplicationEvent::QUIT()));
      queueEvent(QueueEventPtr(new QueueEvent(ae)));
      processEvents(ProcessEventPtr());
    }

  }
}
