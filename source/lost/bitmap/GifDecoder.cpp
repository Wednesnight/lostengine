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

#include "lost/bitmap/GifDecoder.h"
#include "gif_lib.h"
#include "lost/common/Data.h"
#include "lost/resource/Loader.h"
#include "lost/bitmap/Gif.h"

namespace lost
{
namespace bitmap
{

GifDecoder::GifDecoder(const resource::LoaderPtr& inLoader)
{
  _loader = inLoader;
}

GifDecoder::~GifDecoder()
{
}

struct ReadOperation
{
  void* data;
  int offset;
};

int inputfunc(GifFileType* gifFile, GifByteType* buffer, int numBytes)
{
  ReadOperation* op = (ReadOperation*)gifFile->UserData;
  memcpy(buffer, ((char*)op->data)+op->offset, numBytes);
  op->offset += numBytes;
  return numBytes;
}

bool GifDecoder::isGif(const string& path)
{
  bool result = false;
  common::DataPtr data = _loader->load(path);
  ReadOperation op;
  op.data = data->bytes.get();
  op.offset = 0;
  GifFileType* file = DGifOpen(&op, inputfunc);
  if(file)
  {
    result = true;
    DGifCloseFile(file);
  }
  return result;
}

GifPtr GifDecoder::load(const string& path)
{
  common::DataPtr data = _loader->load(path);
  ReadOperation op;
  op.data = data->bytes.get();
  op.offset = 0;
  GifFileType* file = DGifOpen(&op, inputfunc);
  DGifSlurp(file);
  GifPtr result(new Gif(file));
  return result;
}

}
}
