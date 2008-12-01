#include "lost/al/al.h"
#include "lost/al/Listener.h"

using namespace lost::math;

namespace lost
{
namespace al
{
  
void Listener::position(const math::Vec3& val)
{
  alListener3f(AL_POSITION, val.x, val.y, val.z);ALDEBUG_THROW;
}
  
void Listener::velocity(const math::Vec3& val)
{
  alListener3f(AL_VELOCITY, val.x, val.y, val.z);ALDEBUG_THROW;
}

void Listener::gain(float val)
{
  alListenerf(AL_GAIN, val);ALDEBUG_THROW;
}

void Listener::orientation(const math::Vec3& at,
                           const math::Vec3& up)
{
  float o[] = {at.x, at.y, at.z, up.x, up.y, up.z};
  alListenerfv(AL_ORIENTATION, o);ALDEBUG_THROW;
}

math::Vec3 Listener::position()
{
  Vec3 result;
  alGetListener3f(AL_POSITION, &(result.x), &(result.y), &(result.z));ALDEBUG_THROW;
  return result;
}

math::Vec3 Listener::velocity()
{
  Vec3 result;
  alGetListener3f(AL_VELOCITY, &(result.x), &(result.y), &(result.z));ALDEBUG_THROW;
  return result;
}

float Listener::gain()
{
  float result;
  alGetListenerf(AL_GAIN, &result);ALDEBUG_THROW;
  return result;
}

void Listener::getOrientation(math::Vec3& at, math::Vec3& up)
{
  float result[6];
  alGetListenerfv(AL_ORIENTATION, result);ALDEBUG_THROW;
  at.x = result[0];
  at.y = result[1];
  at.z = result[2];
  up.x = result[3];
  up.y = result[4];
  up.z = result[5];
}
  
  
}
}