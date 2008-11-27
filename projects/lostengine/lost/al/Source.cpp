#include "lost/al/al.h"
#include "lost/al/Source.h"

using namespace lost::math;

namespace lost
{
namespace al
{

Source::Source()
{
  alGenSources(1, &source);ALDEBUG_THROW;
}

Source::~Source()
{
  alDeleteSources(1, &source);ALDEBUG; // don't throw
}

void Source::position(const math::Vec3& val)
{
}

void Source::velocity(const math::Vec3& val)
{
}

void Source::gain(float val)
{
}

math::Vec3 Source::position()
{
  Vec3 result;
  
  return result;
}

math::Vec3 Source::velocity()
{
  Vec3 result;
  
  return result;
}

float Source::gain()
{
  return 1.0f;
}


}
}
