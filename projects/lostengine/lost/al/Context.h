#ifndef LOST_AL_CONTEXT_H
#define LOST_AL_CONTEXT_H

#include "lost/al/al.h"

namespace lost
{
namespace al
{
struct Device;

struct Context
{
  Context(lost::shared_ptr<Device> device);
  Context();
  virtual ~Context();

  void initWithDevice(lost::shared_ptr<Device> device);

  void makeCurrent();

  void process();
  void suspend();

  // call these only after you created a context and made it current
  static ALfloat dopplerFactor();
  static ALfloat speedOfSound();
  static ALenum distanceModel();

  static void distanceModel(ALenum dm);

  static string version();
  static string renderer();
  static string vendor();
  static string extensions();

  ALCcontext* context;
  lost::shared_ptr<Device> device;
};
}
}


#endif
