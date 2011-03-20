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

uint32_t Gif::numBitmaps()
{
  return ((GifFileType*)data)->ImageCount;
}

BitmapPtr Gif::paletteAsBitmap()
{
  BitmapPtr result(new Bitmap(256,1,COMPONENTS_RGBA));
  GifFileType* gif = (GifFileType*)data;
  result->clear(Color(0,0,0,0));
  DOUT("cumColors: "<<gif->SColorMap->ColorCount);
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

BitmapPtr Gif::bitmap(uint32_t num)
{
  GifFileType* gif = (GifFileType*)data;
  SavedImage img = gif->SavedImages[num];
  
  uint32_t w = img.ImageDesc.Width;
  uint32_t h = img.ImageDesc.Height;
  uint8_t bgcol = gif->SBackGroundColor;
  DOUT("img w:"<<w<<" h:"<<h);
  
  BitmapPtr result(new Bitmap(w,h,COMPONENTS_RGBA));
  result->clear(Color(0,0,0,0));
  
  for(uint32_t x=0; x<w; ++x)
  {
    for(uint32_t y=0; y<h; ++y)
    {
      uint8_t px = img.RasterBits[y*w+x];
      if(px != bgcol)
      {
        GifColorType col = gif->SColorMap->Colors[px];
        float r = ((float)col.Red)/255.0f;
        float g = ((float)col.Green)/255.0f;
        float b = ((float)col.Blue)/255.0f;
        result->pixel(x,y,Color(r,g,b,1.0));
      }
      else {
        result->pixel(x,y,Color(0,0,0,0.0));        
      }
    }
  }
  result->flip();
  return result;
}

}
}