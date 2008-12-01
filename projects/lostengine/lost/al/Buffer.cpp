#include "lost/al/Buffer.h"

namespace lost
{
namespace al
{

Buffer::Buffer()
{
  alGenBuffers(1, &buffer);ALDEBUG_THROW;
}

Buffer::~Buffer()
{
  alDeleteBuffers(1, &buffer);ALDEBUG; // don't throw
}

void Buffer::bufferData(ALenum format,
                        const ALvoid* data,
                        ALsizei size,
                        ALsizei frequency)
{
  alBufferData(buffer, format, data, size, frequency);ALDEBUG_THROW;
}

}
}