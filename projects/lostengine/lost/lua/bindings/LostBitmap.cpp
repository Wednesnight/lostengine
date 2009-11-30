#include "lost/lua/bindings/LostBitmap.h"
#include "lost/lua/lua.h"

#include "lost/bitmap/Bitmap.h"

using namespace luabind;
using namespace lost::bitmap;

namespace lost
{
  namespace lua
  {

    void LostBitmapBitmap(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("bitmap")
        [
          class_<Bitmap>("Bitmap")
            .def(constructor<>())
            .def(constructor<common::DataPtr>())
            .def_readwrite("width", &Bitmap::width)
            .def_readwrite("height", &Bitmap::height)
        ]
      ];
    }

    void LostBitmap(lua_State* state)
    {
      LostBitmapBitmap(state);
    }
  }
}
