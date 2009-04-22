#include "lost/al/Context.h"
#include <stdexcept>
#include "lost/common/Logger.h"
#include "lost/al/Device.h"

using namespace std;

namespace lost
{
namespace al
{

Context::Context(lost::shared_ptr<Device> inDevice)
{
  initWithDevice(inDevice);
}

Context::Context()
{
  initWithDevice(lost::shared_ptr<Device>(new Device));
}

void Context::initWithDevice(lost::shared_ptr<Device> inDevice)
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

ALfloat Context::dopplerFactor()
{
  ALfloat result = alGetFloat(AL_DOPPLER_FACTOR);ALDEBUG_THROW;
  return result;
}

ALfloat Context::speedOfSound()
{
  ALfloat result = alGetFloat(AL_SPEED_OF_SOUND);ALDEBUG_THROW;
  return result;
}

ALenum Context::distanceModel()
{
  ALenum result = alGetInteger(AL_DISTANCE_MODEL);ALDEBUG_THROW;
  return result;
}

void Context::distanceModel(ALenum dm)
{
  alDistanceModel(dm);ALDEBUG_THROW;
}


std::string Context::version()
{
  const ALchar* result = alGetString(AL_VERSION);ALDEBUG_THROW;
  if(!result)
    throw runtime_error("alGetString returned NULL");
  return string(result);
}

std::string Context::renderer()
{
  const ALchar* result = alGetString(AL_RENDERER);ALDEBUG_THROW;
  if(!result)
    throw runtime_error("alGetString returned NULL");
  return string(result);
}

std::string Context::vendor()
{
  const ALchar* result = alGetString(AL_VENDOR);ALDEBUG_THROW;
  if(!result)
    throw runtime_error("alGetString returned NULL");
  return string(result);
}

std::string Context::extensions()
{
  const ALchar* result = alGetString(AL_EXTENSIONS);ALDEBUG_THROW;
  if(!result)
    throw runtime_error("alGetString returned NULL");
  return string(result);
}

}
}
