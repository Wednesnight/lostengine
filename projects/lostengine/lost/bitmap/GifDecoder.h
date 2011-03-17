#ifndef LOST_BITMAP_GIFDECODER_H
#define LOST_BITMAP_GIFDECODER_H

#include "lost/bitmap/forward.h"
#include "lost/resource/forward.h"

namespace lost
{
namespace bitmap
{

struct GifDecoder
{
  GifDecoder(const resource::LoaderPtr& inLoader);
  virtual ~GifDecoder();

  bool isGif(const std::string& path);
  GifPtr load(const::std::string& path);

private:
  resource::LoaderPtr _loader;
};
}
}

#endif