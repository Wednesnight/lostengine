#ifndef LOST_AUDIO_ENGINE_H
#define LOST_AUDIO_ENGINE_H

#include <boost/shared_ptr.hpp>
#include "lost/al/al.h"
#include "lost/audio/Source.h"

namespace lost
{

namespace al{ struct Context; }
namespace audio
{

struct Engine
{
  Engine();
  ~Engine();
  
//  boost::shared_ptr<audio::Source> createSource();
  
  boost::shared_ptr<al::Context> context;
};

}
}


#endif