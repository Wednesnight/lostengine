#ifndef LOST_AL_BUFFER_H
#define LOST_AL_BUFFER_H

#include "lost/al/al.h"

namespace lost
{
namespace al
{
struct Buffer
{
  Buffer();
  ~Buffer();

  ALint frequency(); // in Hz
  ALint size(); // in Bytes
  ALint bits(); // bits per sample
  ALint channels();

  void bufferData(ALenum format,
                  const ALvoid* data,
                  ALsizei size,
                  ALsizei frequency);

  ALuint buffer;
};
}
}


#endif
