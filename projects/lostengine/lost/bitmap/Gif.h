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

  uint32_t numImages();
  BitmapPtr paletteAsBitmap();
private:
  void* data;
};

}
}

#endif
