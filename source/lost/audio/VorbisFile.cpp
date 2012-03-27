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
