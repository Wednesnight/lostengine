#include "lost/al/Context.h"
#include <stdexcept>
#include "lost/common/Logger.h"

using namespace std;

namespace lost
{
namespace al
{
    
Context::Context(boost::shared_ptr<Device> inDevice)
{
  DOUT("creating context");
  context = alcCreateContext(inDevice->device, NULL);ALDEBUG_THROW;
  if(!context)
    throw runtime_error("couldn't create context for device: "+inDevice->deviceId);
  device = inDevice;
}

Context::~Context()
{
  DOUT("destroying context");
  alcDestroyContext(context);ALDEBUG;
}

void Context::makeCurrent()
{
  DOUT("");
  ALCboolean res = alcMakeContextCurrent(context);ALDEBUG_THROW;
  if(!res)
    throw runtime_error("couldn't make context current");
}

void Context::process()
{
  DOUT("");
  alcProcessContext(context);ALDEBUG_THROW;
}

void Context::suspend()
{
  DOUT("");
  alcSuspendContext(context);ALDEBUG_THROW;
}
  
}
}