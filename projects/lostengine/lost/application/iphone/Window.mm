#include "lost/application/Window.h"
#import "lost/application/iphone/LEWindow.h"
#import "lost/application/iphone/LEGLView.h"
#import "lost/common/Logger.h"

namespace lost
{
namespace application
{
  struct Window::WindowHiddenMembers
  {
    LEWindow* window;
  };


  void Window::initialize()
  {
    DOUT("");
    hiddenMembers = new WindowHiddenMembers;
    hiddenMembers->window = [[LEWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    context.reset(new gl::Context);
    context->defaultFramebuffer(hiddenMembers->window.leglView.defaultFramebuffer);
    open();
  }

  void Window::finalize()
  {
    DOUT("");
    delete hiddenMembers;
  }

  void Window::open()
  {
    DOUT("");
    [hiddenMembers->window makeKeyAndVisible];
  }
  
  void Window::close()
  {
    DOUT("");
  }
}
}