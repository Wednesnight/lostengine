#include "lost/al/al.h"
#include "lost/al/Source.h"
#include "lost/al/Buffer.h"

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
  alSource3f(source, AL_POSITION, val.x, val.y, val.z);ALDEBUG_THROW;
}

void Source::velocity(const math::Vec3& val)
{
  alSource3f(source, AL_VELOCITY, val.x, val.y, val.z);ALDEBUG_THROW;
}

void Source::gain(float val)
{
  alSourcef(source, AL_GAIN, val);ALDEBUG_THROW;
}

math::Vec3 Source::position()
{
  Vec3 result;
  alGetSource3f(source, AL_POSITION, &(result.x), &(result.y), &(result.z));ALDEBUG_THROW;
  return result;
}

math::Vec3 Source::velocity()
{
  Vec3 result;
  alGetSource3f(source, AL_VELOCITY, &(result.x), &(result.y), &(result.z));ALDEBUG_THROW;
  return result;
}

float Source::gain()
{
  float result;
  alGetSourcef(source, AL_GAIN, &result);ALDEBUG_THROW;
  return 1.0f;
}

void Source::relative(bool val)
{
  alSourcei(source, AL_SOURCE_RELATIVE, (val ? AL_TRUE : AL_FALSE));ALDEBUG_THROW;
}

void Source::looping(bool val)
{
  alSourcei(source, AL_LOOPING, (val ? AL_TRUE : AL_FALSE));ALDEBUG_THROW;
}

void Source::buffer(ALint buffer)
{
  alSourcei(source, AL_BUFFER, buffer);ALDEBUG_THROW;
}

void Source::minGain(float val)
{
  alSourcef(source, AL_MIN_GAIN, val);
}

void Source::maxGain(float val)
{
  alSourcef(source, AL_MAX_GAIN, val);
}

void Source::referenceDistance(float val)
{
  alSourcef(source, AL_REFERENCE_DISTANCE, val);ALDEBUG_THROW;
}

void Source::rolloffFactor(float val)
{
  alSourcef(source, AL_ROLLOFF_FACTOR, val);ALDEBUG_THROW;
}

void Source::maxDistance(float val)
{
  alSourcef(source, AL_MAX_DISTANCE, val);ALDEBUG_THROW;
}

void Source::pitch(float val)
{
  alSourcef(source, AL_PITCH, val);ALDEBUG_THROW;
}

void Source::direction(const math::Vec3& val)
{
  alSource3f(source, AL_DIRECTION, val.x, val.y, val.z);ALDEBUG_THROW;
}

void Source::coneInnerAngle(float val)
{
  alSourcef(source, AL_CONE_INNER_ANGLE, val);ALDEBUG_THROW;
}

void Source::coneOuterAngle(float val)
{
  alSourcef(source, AL_CONE_OUTER_ANGLE, val);ALDEBUG_THROW;
}

void Source::coneOuterGain(float val)
{
  alSourcef(source, AL_CONE_OUTER_GAIN, val);ALDEBUG_THROW;
}

void Source::secOffset(float inSeconds)
{
  alSourcef(source, AL_SEC_OFFSET, inSeconds);ALDEBUG_THROW;
}

void Source::sampleOffset(ALint inSampleOffset)
{
  alSourcei(source, AL_SAMPLE_OFFSET, inSampleOffset);ALDEBUG_THROW;
}

void Source::byteOffset(ALint inByteOffset)
{
  alSourcei(source, AL_BYTE_OFFSET, inByteOffset);ALDEBUG_THROW;
}

// --------------------------------------------

bool Source::relative()
{
  bool result;
  ALenum r;
  alGetSourcei(source, AL_SOURCE_RELATIVE, &r);ALDEBUG_THROW;
  result = r ? true : false;
  return result;
}

ALenum Source::type()
{
  ALenum result;
  alGetSourcei(source, AL_SOURCE_TYPE, &result);ALDEBUG_THROW;
  return result;
}

bool Source::looping()
{
  bool result;
  ALenum r;
  alGetSourcei(source, AL_LOOPING, &r);ALDEBUG_THROW;
  result = r ? true : false;
  return result;
}

ALenum Source::state()
{
  ALenum result;
  alGetSourcei(source, AL_SOURCE_STATE, &result);ALDEBUG_THROW;
  return result;
}

ALint Source::queuedBuffers()
{
  ALint result;
  alGetSourcei(source, AL_BUFFERS_QUEUED, &result);ALDEBUG_THROW;
  return result;
}

ALint Source::processedBuffers()
{
  ALint result;
  alGetSourcei(source, AL_BUFFERS_PROCESSED, &result);ALDEBUG_THROW;
  return result;
}

float Source::minGain()
{
  float result;
  alGetSourcef(source, AL_MIN_GAIN, &result);ALDEBUG_THROW;
  return result;
}

float Source::maxGain()
{
  float result;
  alGetSourcef(source, AL_MAX_GAIN, &result);ALDEBUG_THROW;
  return result;
}

float Source::referenceDistance()
{
  float result;
  alGetSourcef(source, AL_REFERENCE_DISTANCE, &result);ALDEBUG_THROW;
  return result;
}

float Source::rolloffFactor()
{
  float result;
  alGetSourcef(source, AL_ROLLOFF_FACTOR, &result);ALDEBUG_THROW;
  return result;
}

float Source::maxDistance()
{
  float result;
  alGetSourcef(source, AL_MAX_DISTANCE, &result);ALDEBUG_THROW;
  return result;
}

float Source::pitch()
{
  float result;
  alGetSourcef(source, AL_PITCH, &result);ALDEBUG_THROW;
  return result;
}

math::Vec3 Source::direction()
{
  math::Vec3 result;
  alGetSource3f(source, AL_DIRECTION, &(result.x), &(result.y), &(result.z));ALDEBUG_THROW;
  return result;
}

float Source::coneInnerAngle()
{
  float result;
  alGetSourcef(source, AL_CONE_INNER_ANGLE, &result);ALDEBUG_THROW;
  return result;
}

float Source::coneOuterAngle()
{
  float result;
  alGetSourcef(source, AL_CONE_OUTER_ANGLE, &result);ALDEBUG_THROW;
  return result;
}

float Source::coneOuterGain()
{
  float result;
  alGetSourcef(source, AL_CONE_OUTER_GAIN, &result);ALDEBUG_THROW;
  return result;
}

void Source::queue(ALuint inBufferId)
{
  alSourceQueueBuffers(source, 1, &inBufferId);ALDEBUG_THROW;
}

void Source::unqueue(ALuint inBufferId)
{
  alSourceUnqueueBuffers(source, 1, &inBufferId);ALDEBUG_THROW;
}

void Source::queue(lost::shared_ptr<Buffer> buffer)
{
  queue(buffer->buffer);
}

void Source::unqueue(lost::shared_ptr<Buffer> buffer)
{
  unqueue(buffer->buffer);
}

void Source::unqueueAllBuffers()
{
	queue(0);
}

void Source::play()
{
  alSourcePlay(source);ALDEBUG_THROW;
}

void Source::pause()
{
  alSourcePause(source);ALDEBUG_THROW;
}

void Source::stop()
{
  alSourceStop(source);ALDEBUG_THROW;
}

void Source::rewind()
{
  alSourceRewind(source);ALDEBUG_THROW;
}

}
}
