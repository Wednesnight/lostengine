#include "lost/application/Window.h"
#include "lost/common/Logger.h"
#include "lost/gl/Context.h"
#include "lost/application/TaskletConfig.h"

#include <boost/thread.hpp>
#include <boost/bind.hpp>

namespace lost
{
  namespace application
  {

    struct Window::WindowHiddenMembers
    {
      lost::shared_ptr<boost::thread> eventThread;
    };

    void Window::initialize()
    {
      DOUT("Window::initialize()");

      // initialize hiddenMembers
      hiddenMembers = new WindowHiddenMembers;

      //

//      hiddenMembers->eventThread.reset(new boost::thread(boost::bind(&linux_::WindowHandler::operator(), hiddenMembers->windowHandler.get())));
    }

    void Window::finalize()
    {
      DOUT("Window::finalize()");

      hiddenMembers->eventThread.reset();
      delete hiddenMembers;
    }

    void Window::open()
    {
      DOUT("Window::open()");
    }

    void Window::close()
    {
      DOUT("Window::close()");
    }

  }
}
