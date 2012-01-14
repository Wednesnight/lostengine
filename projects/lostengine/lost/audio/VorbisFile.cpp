#include "lost/audio/VorbisFile.h"
#include "stb_vorbis.h"
#include <stdexcept>
#include "lost/common/Logger.h"

namespace lost
{
namespace audio
{

VorbisFile::VorbisFile(const common::DataPtr& inFile)
{
	data = NULL;
	int err;
	stb_vorbis* oggfile = stb_vorbis_open_memory((unsigned char*)inFile->bytes.get(), inFile->size, &err, NULL);
	if(!oggfile)
    {
        EOUT("throwing from here");
      lost::common::StringStream os;
        os << "couldn't open ogg file '";
        throw std::runtime_error(os.str().c_str());
    }
	stb_vorbis_info info = stb_vorbis_get_info(oggfile);
	stb_vorbis_close(oggfile);

	sampleRate = info.sample_rate;
	numSamples = stb_vorbis_decode_memory((unsigned char*)(inFile->bytes.get()), inFile->size, &channels, &data);
	DOUT("decoded to "<<numSamples*2 <<" bytes");
  buffer.reset(new al::Buffer);
  buffer->bufferData((channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
                     data,
                     numSamples*2,
                     sampleRate);
}

VorbisFile::~VorbisFile()
{
  buffer.reset();
	if(data)
		free(data);
}

}
}
