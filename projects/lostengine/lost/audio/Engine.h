#ifndef LOST_AUDIO_ENGINE_H
#define LOST_AUDIO_ENGINE_H

#include "lost/al/al.h"
#include <list>
#include <vector>
#include <string>

namespace lost
{

namespace al{ struct Context; }
namespace audio
{

struct Source;

struct Engine : public lost::enable_shared_from_this<Engine>
{
  Engine();
	Engine(const std::string& inDeviceId);
  ~Engine();

	std::vector<std::string> deviceIds;
  lost::shared_ptr<audio::Source> createSource();
  void destroySource(audio::Source* source);

  lost::shared_ptr<al::Context> context;
	std::list<lost::weak_ptr<audio::Source> > sources;

  static lost::shared_ptr<Engine> create()
  {
    return lost::shared_ptr<Engine>(new Engine);
  }
};

}
}


#endif
