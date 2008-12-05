#include "lost/audio/Engine.h"
#include "lost/al/Context.h"
#include "lost/audio/Source.h"
#include "lost/common/Logger.h"

using namespace std;
using namespace boost;

namespace lost
{
namespace audio
{

Engine::Engine()
{
	context.reset(new al::Context());    
	context->makeCurrent();
}

Engine::Engine(const std::string& inDeviceId)
{
	context.reset(new al::Context(boost::shared_ptr<al::Device>(new al::Device(inDeviceId))));    
	context->makeCurrent();
}

Engine::~Engine()
{
	DOUT("sources: "<<sources.size());
}

boost::shared_ptr<audio::Source> Engine::createSource()
{
	if(!selfshit)
		selfshit = shared_from_this();
	shared_ptr<Source> result(new Source);
	
	result->engine = selfshit;
	sources.push_back(result);
	
	return result;
}

void Engine::destroySource(shared_ptr<audio::Source> source)
{
	DOUT("");
	std::list<shared_ptr<Source> >::iterator i;
	for(i=sources.begin(); i!= sources.end(); ++i)
	{
		if(*i == source)
		{
			sources.erase(i);
			break;
		}
	}
}

}
}