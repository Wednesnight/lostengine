#include "lost/audio/Engine.h"
#include "lost/al/Context.h"
#include "lost/al/Device.h"
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
	context.reset(new al::Context(lost::shared_ptr<al::Device>(new al::Device(inDeviceId))));
	context->makeCurrent();
}

Engine::~Engine()
{
	DOUT("sources: "<<sources.size());
	sources.clear();
}

lost::shared_ptr<audio::Source> Engine::createSource()
{
	shared_ptr<Source> result(new Source);

	result->engine = shared_from_this();
	sources.push_back(result);

	return result;
}

void Engine::destroySource(audio::Source* source)
{
	DOUT("");
	std::list<weak_ptr<Source> >::iterator i;
	for(i=sources.begin(); i!= sources.end(); ++i)
	{
		shared_ptr<audio::Source> src = (*i).lock();
		if(src && (src.get() == source)) // erase exact match
		{
			sources.erase(i);
			break;
		}
		else if(!src)	// erase any pointers that aren't alive anymore
		{
			i = sources.erase(i);
		}
	}
}

}
}
