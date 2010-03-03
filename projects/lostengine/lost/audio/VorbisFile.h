#ifndef LOST_AUDIO_VORBISFILE_H
#define LOST_AUDIO_VORBISFILE_H

#include "lost/platform/shared_ptr.h"

namespace lost
{
namespace resource { struct File; };
namespace audio
{

struct VorbisFile
{
	VorbisFile(lost::shared_ptr<resource::File> inFile);
	~VorbisFile();

	int sampleRate;
	int channels;
	int numSamples;
	short* data; // decoompressed data, numsamples*2 bytes
};

}
}

#endif

