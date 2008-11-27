#ifndef LOST_AL_SOURCE_H
#define LOST_AL_SOURCE_H

#include "lost/math/Vec3.h"

namespace lost
{
namespace al
{

// since there's only one listener, all the functions are static
struct Source
{
  Source();
  ~Source();

  // setter
  void position(const math::Vec3& val);
  void velocity(const math::Vec3& val);
  void gain(float val);
  void relative(bool val);
  void looping(bool val);
  void buffer(ALint buffer); // clears queue and sets this buffer as current head
  void minGain(float val);
  void maxGain(float val);
  void referenceGain(float val);
  void referenceDistance(float val);
  void rolloffFactor(float val);
  void maxDistance(float val);
  void pitch(float val);
  void direction(const math::Vec3& val);
  void coneInnerAngle(float val);
  void coneOuterAngle(float val);
  void coneOuterGain(float val);

  // getter
  math::Vec3 position();
  math::Vec3 velocity();
  float gain();
  bool relative();
  ALenum type(); // AL_UNDETERMINED, AL_STATIC, AL_STREAMING
  bool looping();
  ALint queuedBuffers(); // returns the number of currebtly ququed buffers, includes played, playing, not played
  ALint processedBuffers(); // returns the number of processed buffers, indirectly the index of the currently playing buffer
  float minGain();
  float maxGain();
  float referenceGain();
  float referenceDistance();
  float rolloffFactor();
  float maxDistance();
  float pitch();
  math::Vec3 direction();
  float coneInnerAngle();
  float coneOuterAngle();
  float coneOuterGain();
  
  ALuint source;
};
}
}


#endif

