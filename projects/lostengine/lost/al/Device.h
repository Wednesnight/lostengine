#ifndef LOST_AL_DEVICE_H
#define LOST_AL_DEVICE_H

#include "lost/al/al.h"
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

namespace lost
{
namespace al
{
  
  struct Device : private boost::noncopyable
{
  Device(const std::string& inDeviceId);
  virtual ~Device();
  
  static std::vector<std::string> allDeviceIds();
  
  ALCdevice* device;
  std::string deviceId;
};
  
}
}

#endif