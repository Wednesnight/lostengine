#ifndef LOST_AUDIO_ENGINE_H
#define LOST_AUDIO_ENGINE_H

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
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

struct Engine : public boost::enable_shared_from_this<Engine>
{
  Engine();
	Engine(const std::string& inDeviceId);
  ~Engine();
  
	std::vector<std::string> deviceIds;
  boost::shared_ptr<audio::Source> createSource();
  void destroySource(boost::shared_ptr<audio::Source> source);
	
  boost::shared_ptr<al::Context> context;
	std::list<boost::shared_ptr<audio::Source> > sources;
	boost::shared_ptr<Engine> selfshit;
};

}
}


#endif