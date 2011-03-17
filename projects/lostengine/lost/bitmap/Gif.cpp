#include "lost/bitmap/Gif.h"
#include "gif_lib.h"

namespace lost
{
namespace bitmap
{

Gif::Gif(void* inData)
{
  data = inData;
}

Gif::~Gif()
{
  if(data)
  {
    DGifCloseFile((GifFileType*)data);
  }
}

}
}