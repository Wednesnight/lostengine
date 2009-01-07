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
  Device(); // creates a default device
  virtual ~Device();

  void initWithDeviceId(const std::string& inDeviceId);

  static std::vector<std::string> allDeviceIds();
  static std::string defaultDeviceId(); // throws if no default device present

  static void logAllDeviceIds(); // writes all device Ids out via DOUT

  ALCdevice* device;
  std::string deviceId;
};

}
}

#endif
