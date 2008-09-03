#include "lost/lua/bindings/LostBitmapBitmap.h"
#include "lost/bitmap/Bitmap.h"

using namespace luabind;
using namespace lost::bitmap;

LOST_LUA_EXPORT_BEGIN(LostBitmapBitmap)
{
  module(state, "lost")
  [
    namespace_("bitmap")
    [
      class_<Bitmap, boost::shared_ptr<Bitmap> >("Bitmap")
        .def(constructor<>())    
         .def_readwrite("width",     &Bitmap::width)
         .def_readwrite("height",     &Bitmap::height)
    ]
  ];
}
LOST_LUA_EXPORT_END
