#ifndef LOST_APPLICATION_UITASKLET_H
#define LOST_APPLICATION_UITASKLET_H

#include "lost/application/Window.h"
#include "lost/application/Tasklet.h"

namespace lost
{
namespace application
{
struct UiTasklet : public Tasklet
{
  UiTasklet(const WindowParams& params,
            lost::resource::LoaderPtr inLoader= lost::resource::LoaderPtr(new lost::resource::DefaultLoader));
  virtual ~UiTasklet();
  
  bool start();
  
  WindowParams windowParams;
  WindowPtr window;
};
}
}

#endif