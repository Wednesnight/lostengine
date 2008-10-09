#include <boost/shared_ptr.hpp>
#include "lost/lua/State.h"
#include "lost/bitmap/Bitmap.h"

#include "lost/lua/bindings/LostBitmapBitmap.h"

using namespace luabind;
using namespace lost::bitmap;

namespace lost
{
  namespace lua
  {
    void LostBitmapBitmap(lost::lua::State& state)
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
  }
}
