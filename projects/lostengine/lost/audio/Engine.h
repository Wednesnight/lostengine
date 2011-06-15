#ifndef LOST_AUDIO_ENGINE_H
#define LOST_AUDIO_ENGINE_H

#include "lost/al/al.h"

namespace lost
{

namespace al{ struct Context; }
namespace audio
{

struct Source;

struct Engine : public lost::enable_shared_from_this<Engine>
{
  Engine();
	Engine(const string& inDeviceId);
  ~Engine();

	vector<string> deviceIds;
  lost::shared_ptr<audio::Source> createSource();
  void destroySource(audio::Source* source);

  lost::shared_ptr<al::Context> context;
	list<lost::weak_ptr<audio::Source> > sources;

  static lost::shared_ptr<Engine> create()
  {
    return lost::shared_ptr<Engine>(new Engine);
  }
};

}
}


#endif
