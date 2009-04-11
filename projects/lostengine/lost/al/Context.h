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
  Context(boost::shared_ptr<Device> device);
  Context();
  virtual ~Context();

  void initWithDevice(boost::shared_ptr<Device> device);

  void makeCurrent();

  void process();
  void suspend();

  // call these only after you created a context and made it current
  static ALfloat dopplerFactor();
  static ALfloat speedOfSound();
  static ALenum distanceModel();

  static void distanceModel(ALenum dm);

  static std::string version();
  static std::string renderer();
  static std::string vendor();
  static std::string extensions();

  ALCcontext* context;
  boost::shared_ptr<Device> device;
};
}
}


#endif
