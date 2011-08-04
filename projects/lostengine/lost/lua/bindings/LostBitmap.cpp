#include "lost/lua/bindings/LostBitmap.h"
#include "lost/bitmap/Bitmap.h"
#include "lost/bitmap/GifDecoder.h"
#include "lost/bitmap/Gif.h"

#include <slub/slub.h>

using namespace lost::bitmap;
using namespace slub;

namespace lost
{
  namespace lua
  {

    void LostBitmapBitmap(lua_State* state)
    {
      package bitmap = package(state, "lost").package("bitmap");

      bitmap.clazz<Bitmap>("Bitmap")
        .method("clear", &Bitmap::clear)
        .field("width", &Bitmap::width)
        .field("height", &Bitmap::height)
        .method("pixel", (void(Bitmap::*)(uint32_t, uint32_t, const common::Color&))&Bitmap::pixel)
        .method("disc", &Bitmap::disc)
        .method("ring", &Bitmap::ring)
        .method("flip", &Bitmap::flip)
        .method("filledRect", &Bitmap::filledRect)
        .field("premultiplied", &Bitmap::premultiplied)
        .method("premultiplyAlpha", &Bitmap::premultiplyAlpha)
        .method("write", &Bitmap::write)
        .function("create", (BitmapPtr(*)(const common::DataPtr&))&Bitmap::create)
        .function("create", (BitmapPtr(*)(uint32_t, uint32_t, bitmap::Components))&Bitmap::create);

      bitmap
        .enumerated("COMPONENTS_UNDEFINED", lost::bitmap::COMPONENTS_UNDEFINED)
        .enumerated("COMPONENTS_ALPHA", lost::bitmap::COMPONENTS_ALPHA)
        .enumerated("COMPONENTS_RGB", lost::bitmap::COMPONENTS_RGB)
        .enumerated("COMPONENTS_RGBA", lost::bitmap::COMPONENTS_RGBA);
    }

    void LostBitmapGif(lua_State* state)
    {
      package bitmap = package(state, "lost").package("bitmap");

      bitmap.clazz<Gif>("Gif")
        .method("numBitmaps",&Gif::numBitmaps)
        .method("paletteAsBitmap",&Gif::paletteAsBitmap)
        .method("bitmap",&Gif::bitmap);

      bitmap.clazz<GifDecoder>("GifDecoder")
        .constructor<resource::LoaderPtr>()
        .method("isGif",&GifDecoder::isGif)
        .method("load",&GifDecoder::load);
    }

    void LostBitmap(lua_State* state)
    {
      LostBitmapBitmap(state);
      LostBitmapGif(state);
    }
  }
}
