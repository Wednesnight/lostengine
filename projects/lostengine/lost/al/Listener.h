#ifndef LOST_AL_LISTENER_H
#define LOST_AL_LISTENER_H

#include "lost/math/Vec3.h"

namespace lost
{
namespace al
{
  
// since there's only one listener, all the functions are static
struct Listener
{
  static void position(const math::Vec3& val);
  static void velocity(const math::Vec3& val);
  static void gain(float val);
  static void orientation(const math::Vec3& at, const math::Vec3& up);

  static math::Vec3 position();
  static math::Vec3 velocity();
  static float gain();

  static void getOrientation(math::Vec3& at, math::Vec3& up);

};
}
}

#endif

