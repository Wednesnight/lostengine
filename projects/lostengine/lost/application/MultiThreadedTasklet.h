#ifndef LOST_APPLICATION_MULTITHREADEDTASKLET_H
#define LOST_APPLICATION_MULTITHREADEDTASKLET_H

#include "fhtagn/threads/tasklet.h"
#include "lost/application/Tasklet.h"

namespace lost
{
namespace application
{

struct MultiThreadedTasklet : public fhtagn::threads::tasklet, public Tasklet
{
  MultiThreadedTasklet(lost::resource::LoaderPtr inLoader= lost::resource::LoaderPtr(new lost::resource::DefaultLoader));
  virtual ~MultiThreadedTasklet();
  
  void run(fhtagn::threads::tasklet& tasklet);

  void error(fhtagn::threads::tasklet& tasklet, std::exception const& exception);

  virtual bool start(); 
  virtual bool stop();
  virtual bool alive();
  virtual bool wait();
  
};

}
}

#endif