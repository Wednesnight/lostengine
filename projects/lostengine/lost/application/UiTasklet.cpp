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

    UiTasklet::~UiTasklet()
    {
    }

  }
}
