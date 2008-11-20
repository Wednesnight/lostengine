#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/bitmap/Bitmap.h"

#include "lost/lua/bindings/LostBitmapBitmap.h"

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
          class_<Bitmap, boost::shared_ptr<Bitmap> >("Bitmap")
            .def(constructor<>())
            .def(constructor<boost::shared_ptr<lost::resource::File> >())
            .def_readwrite("width", &Bitmap::width)
            .def_readwrite("height", &Bitmap::height)
        ]
      ];
    }
  }
}
