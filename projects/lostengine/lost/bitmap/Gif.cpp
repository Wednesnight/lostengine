#include "lost/bitmap/Bitmap.h"
#include "lost/bitmap/Gif.h"
#include "lost/common/Color.h"
#include "gif_lib.h"
#include "lost/common/Logger.h"

namespace lost
{
namespace bitmap
{

using namespace common;

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

uint32_t Gif::numImages()
{
  return ((GifFileType*)data)->ImageCount;
}

BitmapPtr Gif::paletteAsBitmap()
{
  BitmapPtr result(new Bitmap(256,1,COMPONENTS_RGBA));
  GifFileType* gif = (GifFileType*)data;
  result->clear(Color(0,0,0,0));
  
  if(gif->SColorMap)
  {
    for(uint32_t i=0; i<gif->SColorMap->ColorCount; ++i)
    {
      GifColorType col = gif->SColorMap->Colors[i];
      float r = ((float)col.Red)/255.0f;
      float g = ((float)col.Green)/255.0f;
      float b = ((float)col.Blue)/255.0f;
      result->pixel(i,0,Color(r,g,b,1.0));
    }
  }
  
  return result;
}

}
}