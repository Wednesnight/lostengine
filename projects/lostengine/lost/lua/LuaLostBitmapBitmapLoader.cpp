#include "lost/lua/Luabindings.h"
#include "lost/bitmap/BitmapLoader.h"

using namespace std;
using namespace luabind;
using namespace lost::bitmap;

namespace lost
{
namespace lua
{

void bindLostBitmapBitmapLoader(lost::lua::State& state)
{
  module(state, "lost")
  [
    namespace_("bitmap")
    [
      class_<BitmapLoader >("BitmapLoader")
        .def(constructor<>())    
        .def("loadFromResourcePath", &BitmapLoader::loadFromResourcePath)
    ]
  ];
}

}
}
