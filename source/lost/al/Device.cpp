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

#include <stdexcept>
#include "lost/al/Device.h"
#include "lost/common/Logger.h"

namespace lost
{
namespace al
{

Device::Device(const string& inDeviceId)
{
  initWithDeviceId(inDeviceId);
}

Device::Device()
{
  initWithDeviceId(defaultDeviceId());
}

void Device::initWithDeviceId(const string& inDeviceId)
{
  DOUT("opening device: "<<inDeviceId);
  device = alcOpenDevice(inDeviceId.c_str());ALDEBUG_THROW;
  if(!device)
    THROW_RTE("aclOpenDevice returned NULL for id: "+inDeviceId);
  deviceId = inDeviceId;
}


Device::~Device()
{
  DOUT("closing device: "<<deviceId);
  ALCboolean result = alcCloseDevice(device);ALDEBUG; // don't throw in destructor
  if(!result)
  {
    EOUT("failed to close device: "<<deviceId<<" , did you attach it to a context?");
  }
}

vector<string> Device::allDeviceIds()
{
  vector<string> result;

  const ALCchar* res = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
  while(res && (strlen(res)!=0))
  {
    result.push_back(string(res));
    res+=(strlen(res)+1);
  }

  return result;
}

string Device::defaultDeviceId()
{
  const ALCchar* res = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
  if(!res)
    throw std::runtime_error("no default device found");
  return string(res);
}

void Device::logAllDeviceIds()
{
  vector<string> ids = Device::allDeviceIds();
  DOUT("found "<<ids.size()<<" devices");
  for(unsigned int i = 0; i < ids.size(); ++i)
  {
    DOUT("Device "<<i<<" : '"<<ids[i]<<"'");
  }
}


}
}
