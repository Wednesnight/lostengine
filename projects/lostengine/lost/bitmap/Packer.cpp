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
#include "lost/bitmap/Packer.h"
#include "lost/math/Vec2.h"

using namespace lost::math;

namespace lost
{
namespace bitmap
{

void Packer::Result::clear()
{
  packedBitmap.reset();
  rects.clear();
  bitmapIds.clear();
  rotated.clear();
}


Packer::Packer()
{
}

Packer::~Packer()
{
}

void Packer::pack(Packer::Result& outResult,
        const lost::math::Vec2& targetSize,
        vector<lost::shared_ptr<lost::bitmap::Bitmap> > bitmaps,
        Components format,
        bool rotate,
        bool sort)
{
  outResult.clear();
  buildRectsFromBitmaps(outResult.rects, bitmaps);
  rectPacker.pack(Rect(0,0,targetSize.width, targetSize.height),
                  outResult.rects,
                  rotate,
                  sort);
  // extract missing infos from rectpacker
  // build final bitmap
  outResult.packedBitmap.reset(new Bitmap((uint32_t)targetSize.width, (uint32_t)targetSize.height, format));
  outResult.rects.clear();
  uint32_t numNodes = rectPacker.nodes.size();
  for(uint32_t i=0; i<numNodes; ++i)
  {
    int32_t bmpid = rectPacker.nodes[i].rectid;
    if(bmpid != -1)
    {
      outResult.rects.push_back(rectPacker.nodes[i].rect);
      outResult.bitmapIds.push_back(rectPacker.nodes[i].rectid);
      outResult.rotated.push_back(rectPacker.nodes[i].rotated);

      Rect r = rectPacker.nodes[i].rect;
      shared_ptr<Bitmap> bmp = bitmaps[rectPacker.nodes[i].rectid];
      bmp->draw((uint32_t)r.x, (uint32_t)r.y, outResult.packedBitmap);
//      DOUT("drawing "<<r);
    }
  }
}


void Packer::buildRectsFromBitmaps(vector<lost::math::Rect>& outRects,
                                   vector<lost::shared_ptr<lost::bitmap::Bitmap> > inBitmaps)
{
  outRects.clear();
  uint32_t numBitmaps = inBitmaps.size();
  for(uint32_t i=0; i<numBitmaps; ++i)
  {
    outRects.push_back(Rect(0,0,(float)inBitmaps[i]->width, (float)inBitmaps[i]->height));
  }
}


}
}
