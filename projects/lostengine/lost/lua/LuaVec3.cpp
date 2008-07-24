#include "lost/lua/State.h"
#include "Vec3.h"
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace luabind;
using namespace lost;
using namespace lost::math;

void bindVec3(lost::lua::State& state)
{
	module(state)
	[
	 class_<Vec3, boost::shared_ptr<Vec3> >("Vec3")
	 .def(constructor<>())
	 .def(constructor<float, float, float>())
	 .def("zero", (void(Vec3::*)()) &Vec3::zero)
	 ];
}

// required for shared_ptr management by Lua/luabind
namespace luabind {
  template<class Vec3>
  Vec3* get_pointer(boost::shared_ptr<Vec3>& p) 
  {
    return p.get(); 
  }
	
  template<class Vec3>
  boost::shared_ptr<const Vec3>* 
  get_const_holder(boost::shared_ptr<Vec3>*)
  {
    return 0;
  }
}
