#ifndef LOST_AL_DEVICE_H
#define LOST_AL_DEVICE_H

#include "lost/al/al.h"
#include <boost/noncopyable.hpp>

namespace lost
{
namespace al
{

  struct Device : private boost::noncopyable
{
  Device(const string& inDeviceId);
  Device(); // creates a default device
  virtual ~Device();

  void initWithDeviceId(const string& inDeviceId);

  static vector<string> allDeviceIds();
  static string defaultDeviceId(); // throws if no default device present

  static void logAllDeviceIds(); // writes all device Ids out via DOUT

  ALCdevice* device;
  string deviceId;
};

}
}

#endif
