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

#ifndef LOST_AL_SOURCE_H
#define LOST_AL_SOURCE_H

#include "lost/al/al.h"
#include "lost/math/Vec3.h"

namespace lost
{
namespace al
{

struct Buffer;

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
  void referenceDistance(float val);
  void rolloffFactor(float val);
  void maxDistance(float val);
  void pitch(float val);
  void direction(const math::Vec3& val);
  void coneInnerAngle(float val);
  void coneOuterAngle(float val);
  void coneOuterGain(float val);
  void secOffset(float inSeconds);
  void sampleOffset(ALint inSampleOffset);
  void byteOffset(ALint inByteOffset);

  // getter
  math::Vec3 position();
  math::Vec3 velocity();
  float gain();
  bool relative();
  ALenum type(); // AL_UNDETERMINED, AL_STATIC, AL_STREAMING
  bool looping();
  ALenum state();
  ALint queuedBuffers(); // returns the number of currebtly ququed buffers, includes played, playing, not played
  ALint processedBuffers(); // returns the number of processed buffers, indirectly the index of the currently playing buffer
  float minGain();
  float maxGain();
  float referenceDistance();
  float rolloffFactor();
  float maxDistance();
  float pitch();
  math::Vec3 direction();
  float coneInnerAngle();
  float coneOuterAngle();
  float coneOuterGain();
  
  // playback
  void play();
  void pause();
  void stop();
  void rewind();
  
  // buffer queueing
  void queue(ALuint inBufferId);
  void unqueue(ALuint inBufferId);
	void unqueueAllBuffers();
  void queue(lost::shared_ptr<Buffer> buffer);
  void unqueue(lost::shared_ptr<Buffer> buffer);
  ALuint source;
};
}
}


#endif

