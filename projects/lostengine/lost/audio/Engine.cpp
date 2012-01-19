/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/audio/Engine.h"
#include "lost/al/Context.h"
#include "lost/al/Device.h"
#include "lost/audio/Source.h"
#include "lost/common/Logger.h"

namespace lost
{
namespace audio
{

Engine::Engine()
{
	context.reset(new al::Context());
	context->makeCurrent();
}

Engine::Engine(const string& inDeviceId)
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
	list<weak_ptr<Source> >::iterator i;
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
