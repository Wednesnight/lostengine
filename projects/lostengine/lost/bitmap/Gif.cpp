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
//  DOUT("img w:"<<w<<" h:"<<h);
  
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
