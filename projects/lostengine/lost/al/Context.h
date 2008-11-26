#ifndef LOST_AL_CONTEXT_H
#define LOST_AL_CONTEXT_H

#include "lost/al/al.h"
#include "lost/al/Device.h"
#include <boost/shared_ptr.hpp>

namespace lost
{
namespace al
{
struct Context
{
  Context(boost::shared_ptr<Device> device);
  Context();
  virtual ~Context();
  
  void initWithDevice(boost::shared_ptr<Device> device);
  
  void makeCurrent();

  void process();
  void suspend();
  
  ALCcontext* context;
  boost::shared_ptr<Device> device;
};
}
}


#endif