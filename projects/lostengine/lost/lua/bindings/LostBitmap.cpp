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
            .def_readwrite("width", &Bitmap::width)
            .def_readwrite("height", &Bitmap::height)
            .def("pixel", (void(Bitmap::*)(uint32_t, uint32_t, const common::Color&))&Bitmap::pixel)
            .def("disc", &Bitmap::disc)
            .def("ring", &Bitmap::ring)
            .scope
            [
              def("create", (BitmapPtr(*)(const common::DataPtr&))&Bitmap::create),
              def("create", (BitmapPtr(*)(uint32_t, uint32_t, bitmap::Components))&Bitmap::create)
            ]
        ]
      ];
      globals(state)["lost"]["bitmap"]["COMPONENTS_UNDEFINED"] = lost::bitmap::COMPONENTS_UNDEFINED;
      globals(state)["lost"]["bitmap"]["COMPONENTS_ALPHA"] = lost::bitmap::COMPONENTS_ALPHA;
      globals(state)["lost"]["bitmap"]["COMPONENTS_RGB"] = lost::bitmap::COMPONENTS_RGB;
      globals(state)["lost"]["bitmap"]["COMPONENTS_RGBA"] = lost::bitmap::COMPONENTS_RGBA;
    }

    void LostBitmap(lua_State* state)
    {
      LostBitmapBitmap(state);
    }
  }
}
