#include "lost/lua/Luabindings.h"
#include "lost/bitmap/Bitmap.h"

using namespace std;
using namespace luabind;
using namespace lost::bitmap;


// required for shared_ptr management of Events by Lua/luabind
namespace luabind {
    template<class Bitmap>
    Bitmap* get_pointer(boost::shared_ptr<Bitmap>& p) 
    {
        return p.get(); 
    }

    template<class Bitmap>
    boost::shared_ptr<const Bitmap>* 
    get_const_holder(boost::shared_ptr<Bitmap>*)
    {
        return 0;
    }
}

namespace lost
{
namespace lua
{

void bindLostBitmapBitmap(lost::lua::State& state)
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

