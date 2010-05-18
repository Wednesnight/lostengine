#ifndef LOST_AUDIO_VORBISFILE_H
#define LOST_AUDIO_VORBISFILE_H

#include "lost/platform/shared_ptr.h"
#include "lost/common/Data.h"
#include "lost/al/Buffer.h"

namespace lost
{
namespace resource { struct File; };
namespace audio
{

struct VorbisFile
{
	VorbisFile(const common::DataPtr& inFile);
	~VorbisFile();

	int sampleRate;
	int channels;
	int numSamples;
	short* data; // decoompressed data, numsamples*2 bytes
  shared_ptr<al::Buffer> buffer;
};

}
}

#endif

