#include "lost/audio/Source.h"
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include "lost/audio/VorbisFile.h"
#include "lost/common/Logger.h"

using namespace std;
using namespace boost;

namespace lost
{
namespace audio
{

Source::Source()
{
	DOUT("");
	sourceType = ST_UNDEFINED;
}

Source::~Source()
{
	DOUT("");

	shared_ptr<Engine> ng = engine.lock();
	if(ng)
	{
		ng->destroySource(this);
	}
}

void Source::initWithFile(lost::shared_ptr<resource::File> inFile)
{
	sourceType = ST_STATIC;
	vorbisFile.reset(new VorbisFile(inFile));
}

void Source::play()
{

}

void Source::stop()
{
}

void Source::pause()
{
}

void Source::rewind()
{
}

}
}
