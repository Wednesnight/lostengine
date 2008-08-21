#include "lost/lua/Luabindings.h"
#include "lost/bitmap/BitmapLoader.h"

using namespace luabind;
using namespace lost::bitmap;

namespace lost
{
  namespace lua
  {
    boost::shared_ptr<lost::bitmap::Bitmap> load(BitmapLoader* loader, std::string path)
    {
      return loader->load(path);
    }
  }
}
    
LOST_LUA_EXPORT_BEGIN(LuaLostBitmapBitmapLoader)
{
  module(state, "lost")
  [
    namespace_("bitmap")
    [
      class_<BitmapLoader >("BitmapLoader")
        .def(constructor<boost::shared_ptr<resource::Loader> >())    
        .def("load", &load)
    ]
  ];
}
LOST_LUA_EXPORT_END
