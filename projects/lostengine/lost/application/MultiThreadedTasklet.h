#ifndef LOST_APPLICATION_MULTITHREADEDTASKLET_H
#define LOST_APPLICATION_MULTITHREADEDTASKLET_H

#include "fhtagn/threads/tasklet.h"
#include "lost/application/BasicTasklet.h"

namespace lost
{
namespace application
{

struct MultiThreadedTasklet : public fhtagn::threads::tasklet, public BasicTasklet
{
  MultiThreadedTasklet();
  virtual ~MultiThreadedTasklet();
  
  void run(fhtagn::threads::tasklet& tasklet);

  void error(fhtagn::threads::tasklet& tasklet, std::exception const& exception);

  virtual bool start(); 
  virtual bool stop();
  
};

}
}

#endif