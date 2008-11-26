#include "lost/al/Device.h"
#include "lost/common/Logger.h"

using namespace std;

namespace lost
{
namespace al
{
    
Device::Device(const std::string& inDeviceId)
{
  initWithDeviceId(inDeviceId);
}

Device::Device()
{
  initWithDeviceId(defaultDeviceId());
}

void Device::initWithDeviceId(const std::string& inDeviceId)
{
  DOUT("opening device: "<<inDeviceId);
  device = alcOpenDevice(inDeviceId.c_str());ALDEBUG_THROW;
  if(!device)
    throw runtime_error("aclOpenDevice returned NULL for id: "+inDeviceId);
  deviceId = inDeviceId;
}

  
Device::~Device()
{
  DOUT("closing device: "<<deviceId);
  ALCboolean result = alcCloseDevice(device);ALDEBUG; // don't throw in destructor
  if(!result)
    EOUT("failed to close device: "<<deviceId<<" , did you attach it to a context?");
}

std::vector<std::string> Device::allDeviceIds()
{
  std::vector<std::string> result;

  const ALCchar* res = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
  while(res && (strlen(res)!=0))
  {
    result.push_back(string(res));
    res+=(strlen(res)+1);
  }
  
  return result;
}

std::string Device::defaultDeviceId()
{
  const ALCchar* res = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
  if(!res)
    throw runtime_error("no default device found");
  return string(res);
}

void Device::logAllDeviceIds()
{
  vector<string> ids = Device::allDeviceIds();
  DOUT("found "<<ids.size()<<" devices");
  for(int i=0; i<ids.size(); ++i)
  {
    DOUT("Device "<<i<<" : '"<<ids[i]<<"'");
  }  
}

  
}
}