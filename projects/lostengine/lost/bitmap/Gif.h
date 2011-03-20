#ifndef LOST_BITMAP_GIF_H
#define LOST_BITMAP_GIF_H

#include "lost/bitmap/forward.h"

namespace lost
{
namespace bitmap
{
// don't construct directly, use GifDecoder to load a file
struct Gif
{
  Gif(void* inData);
  virtual ~Gif();

  uint32_t numBitmaps();
  BitmapPtr paletteAsBitmap();
  BitmapPtr bitmap(uint32_t num); // 0-based, returns the specified bitmap as rgba
private:
  void* data;
};

}
}

#endif
