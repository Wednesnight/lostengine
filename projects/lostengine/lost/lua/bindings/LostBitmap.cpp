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
            .def(constructor<const common::DataPtr&>())
            .def_readwrite("width", &Bitmap::width)
            .def_readwrite("height", &Bitmap::height)
            .scope
            [
              def("create", (BitmapPtr(*)(const common::DataPtr&))&Bitmap::create)
            ]
        ]
      ];
    }

    void LostBitmap(lua_State* state)
    {
      LostBitmapBitmap(state);
    }
  }
}
