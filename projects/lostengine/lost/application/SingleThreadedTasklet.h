#ifndef LOST_APPLICATION_SINGLETHREADEDTASKLET_H
#define LOST_APPLICATION_SINGLETHREADEDTASKLET_H

#include "lost/application/Tasklet.h"

namespace lost
{
namespace application
{

struct SingleThreadedTasklet : public Tasklet
{
  SingleThreadedTasklet();
  virtual ~SingleThreadedTasklet();
};

}
}

#endif