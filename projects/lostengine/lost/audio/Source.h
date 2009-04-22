#ifndef LOST_AUDIO_SOURCE_H
#define LOST_AUDIO_SOURCE_H

#include "lost/al/Source.h"
#include "lost/audio/Engine.h"

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
	void initWithFile(lost::shared_ptr<resource::File> inFile);

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
