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