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

#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/Tasklet.h"
#include "lost/application/SpawnTaskletEvent.h"
#include "lost/common/Logger.h"
#include "lost/event/EventDispatcher.h"
#include "lost/platform/Platform.h"

#include <iostream>
#include <stdexcept>

using namespace std;

namespace lost
{
  namespace application
  {

    struct Application::ApplicationHiddenMembers
    {
      DWORD threadId;
    };

    Application* currentApplication = NULL;
    BOOL WINAPI ConsoleHandler(DWORD event)
    {
      if (currentApplication != NULL) currentApplication->quit();
      return TRUE;
    }

    void Application::initialize()
    {
      DOUT("Application::initialize()");

      // setup console handler
      currentApplication = this;
      if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE)
      {
        // unable to install handler for console breaks
        throw runtime_error("SetConsoleCtrlHandler() failed");
      }

      // initialize hiddenMembers
      hiddenMembers = new ApplicationHiddenMembers;
      hiddenMembers->threadId = GetCurrentThreadId();
    }

    void Application::finalize()
    {
      DOUT("Application::finalize()");
      delete hiddenMembers;
    }

    void Application::run(int argc, char *argv[])
    {
      DOUT("Application::run()");

      ApplicationEventPtr event = ApplicationEvent::create(ApplicationEvent::RUN());
      eventDispatcher->dispatchEvent(event);

      while (running && WaitMessage()) {
        MSG msg;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
        eventDispatcher->processEvents();
      }
    }

    void Application::shutdown()
    {
      PostThreadMessage(hiddenMembers->threadId, WM_NULL, 0, 0);
    }

    void Application::showMouse(bool visible)
    {
    }

    void Application::processEvents(const ProcessEventPtr& event)
    {
      PostThreadMessage(hiddenMembers->threadId, WM_NULL, 0, 0);
    }
  }
}
