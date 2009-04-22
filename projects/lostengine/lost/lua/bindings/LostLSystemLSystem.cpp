#include "lost/lua/lua.h"
#include "lost/lsystem/LSystem.h"

#include "lost/lua/bindings/LostLSystemLSystem.h"

using namespace luabind;
using namespace lost::lsystem;

namespace lost
{
  namespace lua
  {
    lost::shared_ptr<lost::lsystem::LSystemState> createLSystemState(const std::string& Axiom, const object& Map, const math::Vec3& Vec)
    {
      std::map<char, std::string> newMap;
      for (luabind::iterator i(Map), end; i != end; ++i)
      {
        std::string key = object_cast<std::string>(i.key());
        std::string value = object_cast<std::string>(*i);
        if (key.size()) newMap[key[0]] = value;
        std::cout << key << " = " << value << std::endl;
      }
      lost::shared_ptr<lsystem::LSystemState> result(new lsystem::LSystemState(Axiom, newMap, Vec));
      return result;
    }
  }
}

namespace lost
{
  namespace lua
  {
    void LostLSystemLSystem(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("lsystem")
        [
          class_<LSystem, lost::shared_ptr<LSystem> >("LSystem")
          .def(constructor<>())
          .def("advance", (void(LSystem::*)(lost::shared_ptr<lsystem::LSystemState>&, unsigned int)) &LSystem::advance)
          .def("advance", (void(LSystem::*)(lost::shared_ptr<lsystem::LSystemState>&)) &LSystem::advance)
          .def("regress", (void(LSystem::*)(lost::shared_ptr<lsystem::LSystemState>&, unsigned int)) &LSystem::regress)
          .def("regress", (void(LSystem::*)(lost::shared_ptr<lsystem::LSystemState>&)) &LSystem::regress)
          .def("walk", (void(LSystem::*)(const lost::shared_ptr<lsystem::LSystemState>&, const lost::math::Matrix&, const boost::function<void (lost::math::Matrix&, LSystemResult&)>)) &LSystem::walk)
        ]
      ];

      module(state, "lost")
      [
        namespace_("lsystem")
        [
          class_<lsystem::LSystemState, lost::shared_ptr<lsystem::LSystemState> >("LSystemState")
          .def("reset", &LSystemState::reset),
          def("LSystemState", &createLSystemState)
        ]
      ];
    }
  }
}
