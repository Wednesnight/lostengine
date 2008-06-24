#include "lost/lua/State.h"
#include "LSystem.h"
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace luabind;
using namespace lost;
using namespace lost::lsystem;

boost::shared_ptr<lsystem::LSystemState> createLSystemState(const std::string& Axiom, const object& Map, const math::Vec3& Vec)
{
  std::map<char, std::string> newMap;
  for (luabind::iterator i(Map), end; i != end; ++i)
  {
    std::string key = object_cast<std::string>(i.key());
    std::string value = object_cast<std::string>(*i);
    if (key.size()) newMap[key[0]] = value;
    std::cout << key << " = " << value << std::endl;
  }
  boost::shared_ptr<lsystem::LSystemState> result(new lsystem::LSystemState(Axiom, newMap, Vec));
  return result;
}

void bindLSystem(lost::lua::State& state)
{
	module(state)
	[
	 class_<LSystem, boost::shared_ptr<LSystem> >("LSystem")
	 .def(constructor<>())
	 .def("advance", (void(LSystem::*)(boost::shared_ptr<lsystem::LSystemState>&, unsigned int)) &LSystem::advance)
	 .def("advance", (void(LSystem::*)(boost::shared_ptr<lsystem::LSystemState>&)) &LSystem::advance)
	 .def("regress", (void(LSystem::*)(boost::shared_ptr<lsystem::LSystemState>&, unsigned int)) &LSystem::regress)
	 .def("regress", (void(LSystem::*)(boost::shared_ptr<lsystem::LSystemState>&)) &LSystem::regress)
	 .def("walk", (void(LSystem::*)(const boost::shared_ptr<lsystem::LSystemState>&, const lost::math::Matrix&, const boost::function<void (lost::math::Matrix&, LSystemResult&)>)) &LSystem::walk)
	 ];

	module(state)
	[
	 class_<lsystem::LSystemState, boost::shared_ptr<lsystem::LSystemState> >("LSystemState")
   .def("reset", &LSystemState::reset),
   def("LSystemState", &createLSystemState)
	 ];
}

// required for shared_ptr management by Lua/luabind
namespace luabind {
    template<class LSystem>
    LSystem* get_pointer(boost::shared_ptr<LSystem>& p) 
    {
        return p.get(); 
    }
	
    template<class LSystem>
    boost::shared_ptr<const LSystem>* 
    get_const_holder(boost::shared_ptr<LSystem>*)
    {
        return 0;
    }
}
