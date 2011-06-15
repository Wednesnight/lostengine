#include "lost/audio/Source.h"
#include "lost/audio/VorbisFile.h"
#include "lost/common/Logger.h"

using namespace boost;

namespace lost
{
namespace audio
{

Source::Source()
{
	DOUT("");
	sourceType = ST_UNDEFINED;
  source.reset(new al::Source);
}

Source::~Source()
{
	DOUT("");

  source->stop();
  if (vorbisFile) source->unqueue(vorbisFile->buffer);
	shared_ptr<Engine> ng = engine.lock();
	if(ng)
	{
		ng->destroySource(this);
	}
}

void Source::initWithFile(const common::DataPtr& inFile)
{
	sourceType = ST_STATIC;
	vorbisFile.reset(new VorbisFile(inFile));
  source->queue(vorbisFile->buffer);
}

void Source::play()
{
  source->play();
}

void Source::stop()
{
  source->stop();
}

void Source::pause()
{
  source->pause();
}

void Source::rewind()
{
  source->rewind();
}

}
}
