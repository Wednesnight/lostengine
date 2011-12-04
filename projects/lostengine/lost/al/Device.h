#ifndef LOST_AL_DEVICE_H
#define LOST_AL_DEVICE_H

#include "lost/common/Disallow.h"
#include "lost/al/al.h"

namespace lost
{
namespace al
{
struct Device
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

  private:
    DISALLOW_COPY_AND_ASSIGN(Device);
  
};

}
}

#endif
