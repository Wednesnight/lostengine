#ifndef LOST_BITMAP_GIF_H
#define LOST_BITMAP_GIF_H

namespace lost
{
namespace bitmap
{
// don't construct directly, use GifDecoder to load a file
struct Gif
{
  Gif(void* inData);
  virtual ~Gif();

private:
  void* data;
};

}
}

#endif
