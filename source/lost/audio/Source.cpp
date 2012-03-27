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

#include "lost/audio/Source.h"
#include "lost/audio/VorbisFile.h"
#include "lost/common/Logger.h"

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
