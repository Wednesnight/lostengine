#include "lost/bitmap/Bitmap.h"
#include "lost/bitmap/Packer.h"

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
        std::vector<lost::shared_ptr<lost::bitmap::Bitmap> > bitmaps,
        Bitmap::Components format,
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


void Packer::buildRectsFromBitmaps(std::vector<lost::math::Rect>& outRects,
                                   std::vector<lost::shared_ptr<lost::bitmap::Bitmap> > inBitmaps)
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
