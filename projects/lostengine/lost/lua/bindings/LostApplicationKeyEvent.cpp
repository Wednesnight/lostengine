#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/KeySym.h"
#include "lost/lua/EventCast.h"

#include "lost/lua/bindings/LostApplicationKeyEvent.h"

using namespace luabind;
using namespace lost::application;
using namespace lost::event;

namespace lost
{
  namespace lua
  {
    void LostApplicationKeyEvent(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<KeyEvent, Event, boost::shared_ptr<KeyEvent> >("KeyEvent")
          .def(constructor<std::string>()) 
          .def_readwrite("key", &KeyEvent::key)
          .def_readwrite("character", &KeyEvent::character)
          .def_readwrite("pressed", &KeyEvent::pressed)
          .def_readwrite("repeat", &KeyEvent::repeat)
          .scope
          [
            def("cast", &lost::lua::cast<KeyEvent>)
          ]
        ]
      ];
      globals(state)["lost"]["application"]["KeyEvent"]["KEY_UP"] = KeyEvent::KEY_UP();
      globals(state)["lost"]["application"]["KeyEvent"]["KEY_DOWN"] = KeyEvent::KEY_DOWN();

      globals(state)["lost"]["application"]["K_UNKNOWN"] = K_UNKNOWN;

      globals(state)["lost"]["application"]["K_BACKSPACE"] = K_BACKSPACE;
      globals(state)["lost"]["application"]["K_TAB"]       = K_TAB;
      globals(state)["lost"]["application"]["K_ENTER"]     = K_ENTER;
      globals(state)["lost"]["application"]["K_PAUSE"]     = K_PAUSE;
      globals(state)["lost"]["application"]["K_ESCAPE"]    = K_ESCAPE;
      globals(state)["lost"]["application"]["K_SPACE"]     = K_SPACE;

      globals(state)["lost"]["application"]["K_LEFT"]  = K_LEFT;
      globals(state)["lost"]["application"]["K_UP"]    = K_UP;
      globals(state)["lost"]["application"]["K_RIGHT"] = K_RIGHT;
      globals(state)["lost"]["application"]["K_DOWN"]  = K_DOWN;

      globals(state)["lost"]["application"]["K_0"] = K_0;
      globals(state)["lost"]["application"]["K_1"] = K_1;
      globals(state)["lost"]["application"]["K_2"] = K_2;
      globals(state)["lost"]["application"]["K_3"] = K_3;
      globals(state)["lost"]["application"]["K_4"] = K_4;
      globals(state)["lost"]["application"]["K_5"] = K_5;
      globals(state)["lost"]["application"]["K_6"] = K_6;
      globals(state)["lost"]["application"]["K_7"] = K_7;
      globals(state)["lost"]["application"]["K_8"] = K_8;
      globals(state)["lost"]["application"]["K_9"] = K_9;

      globals(state)["lost"]["application"]["K_A"] = K_A;
      globals(state)["lost"]["application"]["K_B"] = K_B;
      globals(state)["lost"]["application"]["K_C"] = K_C;
      globals(state)["lost"]["application"]["K_D"] = K_D;
      globals(state)["lost"]["application"]["K_E"] = K_E;
      globals(state)["lost"]["application"]["K_F"] = K_F;
      globals(state)["lost"]["application"]["K_G"] = K_G;
      globals(state)["lost"]["application"]["K_H"] = K_H;
      globals(state)["lost"]["application"]["K_I"] = K_I;
      globals(state)["lost"]["application"]["K_J"] = K_J;
      globals(state)["lost"]["application"]["K_K"] = K_K;
      globals(state)["lost"]["application"]["K_L"] = K_L;
      globals(state)["lost"]["application"]["K_M"] = K_M;
      globals(state)["lost"]["application"]["K_N"] = K_N;
      globals(state)["lost"]["application"]["K_O"] = K_O;
      globals(state)["lost"]["application"]["K_P"] = K_P;
      globals(state)["lost"]["application"]["K_Q"] = K_Q;
      globals(state)["lost"]["application"]["K_R"] = K_R;
      globals(state)["lost"]["application"]["K_S"] = K_S;
      globals(state)["lost"]["application"]["K_T"] = K_T;
      globals(state)["lost"]["application"]["K_U"] = K_U;
      globals(state)["lost"]["application"]["K_V"] = K_V;
      globals(state)["lost"]["application"]["K_W"] = K_W;
      globals(state)["lost"]["application"]["K_X"] = K_X;
      globals(state)["lost"]["application"]["K_Y"] = K_Y;
      globals(state)["lost"]["application"]["K_Z"] = K_Z;

      globals(state)["lost"]["application"]["K_NUMPAD_0"] = K_NUMPAD_0;
      globals(state)["lost"]["application"]["K_NUMPAD_1"] = K_NUMPAD_1;
      globals(state)["lost"]["application"]["K_NUMPAD_2"] = K_NUMPAD_2;
      globals(state)["lost"]["application"]["K_NUMPAD_3"] = K_NUMPAD_3;
      globals(state)["lost"]["application"]["K_NUMPAD_4"] = K_NUMPAD_4;
      globals(state)["lost"]["application"]["K_NUMPAD_5"] = K_NUMPAD_5;
      globals(state)["lost"]["application"]["K_NUMPAD_6"] = K_NUMPAD_6;
      globals(state)["lost"]["application"]["K_NUMPAD_7"] = K_NUMPAD_7;
      globals(state)["lost"]["application"]["K_NUMPAD_8"] = K_NUMPAD_8;
      globals(state)["lost"]["application"]["K_NUMPAD_9"] = K_NUMPAD_9;

      globals(state)["lost"]["application"]["K_NUMPAD_PLUS"]  = K_NUMPAD_PLUS;
      globals(state)["lost"]["application"]["K_NUMPAD_MINUS"] = K_NUMPAD_MINUS;

      globals(state)["lost"]["application"]["K_F1"]  = K_F1;
      globals(state)["lost"]["application"]["K_F2"]  = K_F2;
      globals(state)["lost"]["application"]["K_F3"]  = K_F3;
      globals(state)["lost"]["application"]["K_F4"]  = K_F4;
      globals(state)["lost"]["application"]["K_F5"]  = K_F5;
      globals(state)["lost"]["application"]["K_F6"]  = K_F6;
      globals(state)["lost"]["application"]["K_F7"]  = K_F7;
      globals(state)["lost"]["application"]["K_F8"]  = K_F8;
      globals(state)["lost"]["application"]["K_F9"]  = K_F9;
      globals(state)["lost"]["application"]["K_F10"] = K_F10;
      globals(state)["lost"]["application"]["K_F11"] = K_F11;
      globals(state)["lost"]["application"]["K_F12"] = K_F12;

      globals(state)["lost"]["application"]["K_DELETE"] = K_DELETE;
      globals(state)["lost"]["application"]["K_INSERT"] = K_INSERT;

      globals(state)["lost"]["application"]["K_HOME"]     = K_HOME;
      globals(state)["lost"]["application"]["K_END"]      = K_END;
      globals(state)["lost"]["application"]["K_PAGEUP"]   = K_PAGEUP;
      globals(state)["lost"]["application"]["K_PAGEDOWN"] = K_PAGEDOWN;

      globals(state)["lost"]["application"]["K_RSHIFT"] = K_RSHIFT;
      globals(state)["lost"]["application"]["K_LSHIFT"] = K_LSHIFT;
      globals(state)["lost"]["application"]["K_RCTRL"]  = K_RCTRL;
      globals(state)["lost"]["application"]["K_LCTRL"]  = K_LCTRL;
    }
  }
}
