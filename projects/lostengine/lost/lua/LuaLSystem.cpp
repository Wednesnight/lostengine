#include "lost/lua/Luabindings.h"
#include "lost/lsystem/LSystem.h"

using namespace luabind;
using namespace lost::lsystem;

namespace lost
{
  namespace lua
  {
    boost::shared_ptr<lost::lsystem::LSystemState> createLSystemState(const std::string& Axiom, const object& Map, const math::Vec3& Vec)
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
  }
}

LOST_LUA_EXPORT_BEGIN(LuaLostLSystemLSystem)
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
LOST_LUA_EXPORT_END
