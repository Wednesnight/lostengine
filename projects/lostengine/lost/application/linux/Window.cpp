#include "lost/application/Window.h"
#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/WindowEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/DragNDropEvent.h"
#include "lost/common/Logger.h"
#include "lost/gl/Context.h"
#include "lost/application/TaskletConfig.h"
#include <map>

namespace lost
{
  namespace application
  {

    struct Window::WindowHiddenMembers
    {
    };

    void Window::initialize()
    {
      DOUT("Window::initialize()");

      // initialize hiddenMembers
      hiddenMembers = new WindowHiddenMembers;

      // TODO
    }

    void Window::finalize()
    {
      DOUT("Window::finalize()");

      // TODO

      delete hiddenMembers;
    }

    void Window::open()
    {
      DOUT("Window::open()");
      // TODO
    }

    void Window::close()
    {
      DOUT("Window::close()");
      // TODO
    }

  }
}

