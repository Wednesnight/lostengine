#include "lost/application/UiTasklet.h"

namespace lost
{
  namespace application
  {

    UiTasklet::UiTasklet(lost::resource::LoaderPtr inLoader)
    {
    }

    UiTasklet::UiTasklet(const WindowParams& params, lost::resource::LoaderPtr inLoader)
    : windowParams(params)
    {
    }

    bool UiTasklet::start()
    {
      window = Window::create(eventDispatcher, windowParams);
      window->open();
      return Tasklet::start();
    }

    void UiTasklet::init()
    {
      // make sure that our GL context is the current context
      window->context->makeCurrent();
    }

    UiTasklet::~UiTasklet()
    {
    }

  }
}
