#ifndef LOST_APPLICATION_SINGLETHREADEDTASKLET_H
#define LOST_APPLICATION_SINGLETHREADEDTASKLET_H

#include "lost/application/Tasklet.h"

namespace lost
{
namespace application
{

struct SingleThreadedTasklet : public Tasklet
{
  SingleThreadedTasklet(lost::resource::LoaderPtr inLoader= lost::resource::LoaderPtr(new lost::resource::DefaultLoader));
  virtual ~SingleThreadedTasklet();
};

}
}

#endif