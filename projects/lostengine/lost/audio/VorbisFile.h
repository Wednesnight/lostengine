#ifndef LOST_AUDIO_VORBISFILE:H
#define LOST_AUDIO_VORBISFILE

#include <boost/shared_ptr.hpp>

namespace lost
{
namespace resource { struct File; };
namespace audio
{

struct VorbisFile
{
	VorbisFile(boost::shared_ptr<resource::File> inFile);
	~VorbisFile();
	
	int sampleRate;
	int channels;
	int numSamples;
	short* data; // decoompressed data, numsamples*2 bytes
};

}
}

#endif

