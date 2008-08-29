#include "lost/lua/Luabindings.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/KeySym.h"
#include "lost/lua/LuaEventCast.h"

using namespace luabind;
using namespace lost::application;
using namespace lost::event;

LOST_LUA_EXPORT_BEGIN(LuaLostApplicationKeyEvent)
{
  module(state, "lost")
  [
    namespace_("application")
    [
      class_<KeyEvent, boost::shared_ptr<KeyEvent>, boost::shared_ptr<Event> >("KeyEvent")
      .def(constructor<std::string>()) 
      .def_readwrite("key", &KeyEvent::key)
      .def_readwrite("pressed", &KeyEvent::pressed)
      .scope
      [
        def("cast", &lost::lua::cast<KeyEvent>)
      ]
    ]
  ];
  globals(state)["lost"]["application"]["KeyEvent"]["KEY_UP"] = KeyEvent::KEY_UP();
  globals(state)["lost"]["application"]["KeyEvent"]["KEY_DOWN"] = KeyEvent::KEY_DOWN();

  globals(state)["lost"]["application"]["K_ESCAPE"] = K_ESCAPE;
}
LOST_LUA_EXPORT_END
