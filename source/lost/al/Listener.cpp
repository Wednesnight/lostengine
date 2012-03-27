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
