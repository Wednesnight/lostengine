#include "lost/lua/State.h"
#include "lost/math/Vec4.h"
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace luabind;
using namespace lost;
using namespace lost::math;

namespace lost
{
  namespace lua
  {
    
    void bindLostMathVec4(lost::lua::State& state)
    {
      module(state, "lost")
      [
        namespace_("math")
        [
          class_<Vec4, boost::shared_ptr<Vec4> >("Vec4")
            .def(constructor<>())
            .def(constructor<float, float, float, float>())
            .def("clear", (void(Vec4::*)()) &Vec4::clear)
        ]
      ];
    }

  }
}

// required for shared_ptr management by Lua/luabind
namespace luabind {
  template<class Vec4>
  Vec4* get_pointer(boost::shared_ptr<Vec4>& p) 
  {
    return p.get(); 
  }
	
  template<class Vec4>
  boost::shared_ptr<const Vec4>* 
  get_const_holder(boost::shared_ptr<Vec4>*)
  {
    return 0;
  }
}
