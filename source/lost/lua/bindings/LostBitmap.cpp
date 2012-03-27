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

#include "lost/lua/bindings/LostBitmap.h"
#include "lost/bitmap/Bitmap.h"
#include "lost/bitmap/GifDecoder.h"
#include "lost/bitmap/Gif.h"
#include "lost/common/Data.h"
#include "lost/resource/Loader.h"

#include <slub/slub.h>

using namespace lost::bitmap;

namespace lost
{
  namespace lua
  {

    void LostBitmapBitmap(lua_State* state)
    {
      slub::package bitmap = slub::package(state, "lost").package("bitmap");

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
      slub::package bitmap = slub::package(state, "lost").package("bitmap");

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
