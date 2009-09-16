#ifndef LOST_APPLICATION_ACCELEROMETEREVENT_H
#define LOST_APPLICATION_ACCELEROMETEREVENT_H

#include "lost/event/Event.h"

namespace lost
{
template <typename T>
struct shared_ptr;
namespace application
{
struct AccelerometerEvent;
typedef lost::shared_ptr<AccelerometerEvent> AccelerometerEventPtr;  

// sent on iphone/ipod touch when accelerometer is active and the device is moved
struct AccelerometerEvent : public event::Event
{
  // x,y,z contain estimated acceleration values along the given axis, where a value of 1
  // means an acceleration of 1g along the given axis
  double x;
  double y;
  double z;
  // FIXME: timestamp will probably be provided as CFAbsoluteTime with baseline at 2001
  // we must not mix this up with gettimeofday with baseline at 1970 and should unify
  // the time api and maybe base lost::platform::currentTime on
  double timeStamp;

  static const event::Type& DEVICE_ACCELERATED() { static event::Type d = "deviceAccelerated";return d;}

  AccelerometerEvent(const event::Type& inType) : Event(inType) { }
  virtual ~AccelerometerEvent() {}

};

}
}

#endif
