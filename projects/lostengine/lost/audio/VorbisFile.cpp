#include "lost/audio/VorbisFile.h"
#include "stb_vorbis.h"
#include "lost/resource/File.h"
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include "lost/common/Logger.h"

using namespace std;
using namespace boost;

namespace lost
{
namespace audio
{

VorbisFile::VorbisFile(boost::shared_ptr<resource::File> inFile)
{
	data = NULL;
	int err;
	stb_vorbis* oggfile = stb_vorbis_open_memory((unsigned char*)inFile->data.get(), inFile->size, &err, NULL);
	if(!oggfile)
		throw runtime_error(string("couldn't open ogg file '")+inFile->location+"' with error: "+lexical_cast<string>(err));
	stb_vorbis_info info = stb_vorbis_get_info(oggfile);
	stb_vorbis_close(oggfile);

	sampleRate = info.sample_rate;
	numSamples = stb_vorbis_decode_memory((unsigned char*)(inFile->data.get()), inFile->size, &channels, &data);
	DOUT("decoded to "<<numSamples*2 <<" bytes");
}

VorbisFile::~VorbisFile()
{
	if(data)
		free(data);
}

}
}
