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

#ifndef LOST_AUDIO_SOURCE_H
#define LOST_AUDIO_SOURCE_H

#include "lost/al/Source.h"
#include "lost/audio/Engine.h"
#include "lost/common/Data.h"

namespace lost
{
namespace resource {struct File;}
namespace audio
{

struct Engine;
struct VorbisFile;

// Sources are basically OpenAL sources with additional helper code
// for playing static and streaming audio.
struct Source : public lost::enable_shared_from_this<Source>
{
	enum SourceType
	{
		ST_UNDEFINED = 0,
		ST_STATIC = 1,
		ST_STREAMING=2,
		ST_NUM
	};

	virtual ~Source();

	// initialises this source with a static chunk of data
	// the source will decompress the audio if necessary and keep a copy
	// so you can discard the file afterwards.
	// NOTE: this currently only supports ogg files.
	void initWithFile(const common::DataPtr& inFile);

	void play();
	void stop();
	void pause();
	void rewind();

	lost::weak_ptr<Engine> engine;
	lost::shared_ptr<al::Source> source;

	SourceType sourceType;

	lost::shared_ptr<VorbisFile> vorbisFile; //  for static playback
private:
	friend struct Engine;
	Source(); // use the audio engine to create sources

};

}
}

#endif
