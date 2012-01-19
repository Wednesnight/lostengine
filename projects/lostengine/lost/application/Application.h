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

#ifndef LOST_APPLICATION_APPLICATION_H
#define LOST_APPLICATION_APPLICATION_H

#include "lost/event/forward.h"
#include "lost/application/forward.h"

namespace lost
{
  namespace application
  {
    struct Application 
    {
    private:
      /**
       * forward declaration for platform specific stuff
       */
      struct ApplicationHiddenMembers;
      ApplicationHiddenMembers* hiddenMembers;

      bool running;

      /**
       * hidden constructor
       */
      Application(Tasklet* tasklet);
      
      /**
       * hidden ctor/dtor utility methods for platform specific stuff
       */
      void initialize();
      void finalize();

      /**
       * Removes the specified tasklet.
       */
      void removeTasklet(Tasklet * tasklet);


      /**
       * listener for ApplicationEvent.RUN()
       * handled in generic code
       * raised from platform specific code
       */
      void startup(const ApplicationEventPtr& event);

      /**
       * listener for ApplicationEvent.QUIT()
       */
      void quitHandler(const ApplicationEventPtr& event);

      /**
       * handled in platform specific code
       * raised from generic code
       * should be executed on main thread!
       */
      void shutdown();

      void taskletSpawn(const SpawnTaskletEventPtr& event);   // Tasklet spawn handler
      void taskletTerminate(const TaskletEventPtr& event);    // Tasklet terminate handler
      void taskletDone(const TaskletEventPtr& event);         // Tasklet done handler
      void queueEvent(const QueueEventPtr& event);            // listener for QueueEvent.QUEUE()
      void processEvents(const ProcessEventPtr& event);       // should be executed on main thread!

    public:
      /**
       * list of tasklets
       * see: lost/application/Tasklet.h
       */
      list<Tasklet*> tasklets;
      
      /**
       * see: lost/event/EventDispatcher.h
       */
      lost::event::EventDispatcherPtr eventDispatcher;

      /**
       * returns Application singleton
       */
      static ApplicationPtr getInstance(Tasklet* tasklet = NULL);

      ~Application();

      void addTasklet(Tasklet* tasklet);  // adds the specified tasklet, calls it's init() and start() methods
      void run(int argc, char *argv[]);   // starts all tasklets and queues a run event
      void quit();                        // quits the app
      void showMouse(bool visible);       // call this to show/hide the OS mouse
    };

  }
}

#endif
