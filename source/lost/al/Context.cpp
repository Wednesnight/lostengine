/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/al/Context.h"
#include <stdexcept>
#include "lost/common/Logger.h"
#include "lost/al/Device.h"

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
    THROW_RTE("couldn't create context for device: "+inDevice->deviceId);
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
    throw std::runtime_error("couldn't make context current");
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


string Context::version()
{
  const ALchar* result = alGetString(AL_VERSION);ALDEBUG_THROW;
  if(!result)
    throw std::runtime_error("alGetString returned NULL");
  return string(result);
}

string Context::renderer()
{
  const ALchar* result = alGetString(AL_RENDERER);ALDEBUG_THROW;
  if(!result)
    throw std::runtime_error("alGetString returned NULL");
  return string(result);
}

string Context::vendor()
{
  const ALchar* result = alGetString(AL_VENDOR);ALDEBUG_THROW;
  if(!result)
    throw std::runtime_error("alGetString returned NULL");
  return string(result);
}

string Context::extensions()
{
  const ALchar* result = alGetString(AL_EXTENSIONS);ALDEBUG_THROW;
  if(!result)
    throw std::runtime_error("alGetString returned NULL");
  return string(result);
}

}
}
