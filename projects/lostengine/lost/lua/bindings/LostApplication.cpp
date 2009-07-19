#include "lost/lua/bindings/LostApplication.h"
#include "lost/lua/lua.h"
#include "lost/lua/EventCast.h"
#include <luabind/iterator_policy.hpp>

#include "lost/application/AccelerometerEvent.h"
#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/InputEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/Tasklet.h"
#include "lost/application/TouchEvent.h"
#include "lost/application/Window.h"

using namespace luabind;
using namespace lost::application;
using namespace lost::event;

namespace lost
{
  namespace lua
  {

    void LostApplicationAccelerometerEvent(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<AccelerometerEvent, Event, AccelerometerEventPtr>("AccelerometerEvent")
            .def(constructor<std::string>()) 
            .def_readwrite("x", &AccelerometerEvent::x)
            .def_readwrite("y", &AccelerometerEvent::y)
            .def_readwrite("z", &AccelerometerEvent::z)
            .def_readwrite("timeStamp", &AccelerometerEvent::timeStamp)
            .scope
            [
              def("cast", &lost::lua::cast<AccelerometerEvent>)
            ]
        ]
      ];
      globals(state)["lost"]["application"]["AccelerometerEvent"]["DEVICE_ACCELERATED"] = AccelerometerEvent::DEVICE_ACCELERATED();
    }

    void LostApplicationApplication(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<Application, ApplicationPtr>("Application")
            .def("quit", &Application::quit)
            .def("showMouse", &Application::showMouse)
            .def_readonly("loader", &Application::loader)
            .def_readonly("eventDispatcher", &Application::eventDispatcher)
        ]
      ];
    }
    
    void LostApplicationApplicationEvent(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<ApplicationEvent, Event, lost::shared_ptr<Event> >("ApplicationEvent")
            .def(constructor<std::string>()) 
            .scope
            [
              def("cast", &lost::lua::cast<ApplicationEvent>)
            ]
        ]
      ];
      globals(state)["lost"]["application"]["ApplicationEvent"]["RUN"] = ApplicationEvent::RUN();
      globals(state)["lost"]["application"]["ApplicationEvent"]["QUIT"] = ApplicationEvent::QUIT();
    }

    void LostApplicationInputEvent(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<InputEvent, Event, lost::shared_ptr<InputEvent> >("InputEvent")
            .def(constructor<std::string>()) 
            .def_readonly("window", &InputEvent::window)
            .scope
            [
              def("cast", &lost::lua::cast<InputEvent>)
            ]
        ]
      ];
    }

    void LostApplicationKeyEvent(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<KeyEvent, InputEvent, lost::shared_ptr<KeyEvent> >("KeyEvent")
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

    void LostApplicationMouseEvent(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<MouseEvent, InputEvent, lost::shared_ptr<MouseEvent> >("MouseEvent")
            .def(constructor<std::string>()) 
            .def_readwrite("pos", &MouseEvent::pos)
            .def_readwrite("absPos", &MouseEvent::absPos)
            .def_readwrite("button", &MouseEvent::button)
            .def_readwrite("pressed", &MouseEvent::pressed)
            .scope
            [
              def("cast", &lost::lua::cast<MouseEvent>)
            ]
        ]
      ];
      globals(state)["lost"]["application"]["MouseEvent"]["MOUSE_UP"] = MouseEvent::MOUSE_UP();
      globals(state)["lost"]["application"]["MouseEvent"]["MOUSE_DOWN"] = MouseEvent::MOUSE_DOWN();
      globals(state)["lost"]["application"]["MouseEvent"]["MOUSE_MOVE"] = MouseEvent::MOUSE_MOVE();
      
      globals(state)["lost"]["application"]["MB_UNKNOWN"] = MB_UNKNOWN;
      
      globals(state)["lost"]["application"]["MB_LEFT"]   = MB_LEFT;
      globals(state)["lost"]["application"]["MB_RIGHT"]  = MB_RIGHT;
      globals(state)["lost"]["application"]["MB_MIDDLE"] = MB_MIDDLE;
    }

    void LostApplicationResizeEvent(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<ResizeEvent, Event, lost::shared_ptr<ResizeEvent> >("ResizeEvent")
            .def(constructor<std::string>()) 
            .def_readwrite("width", &ResizeEvent::width)
            .def_readwrite("height", &ResizeEvent::height)
            .scope
            [
              def("cast", &lost::lua::cast<ResizeEvent>)
            ]
        ]
      ];
      globals(state)["lost"]["application"]["ResizeEvent"]["MAIN_WINDOW_RESIZE"] = ResizeEvent::MAIN_WINDOW_RESIZE();
    }

    void LostApplicationTasklet(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<Tasklet, TaskletPtr>("Tasklet")
            .def_readonly("eventDispatcher", &Tasklet::eventDispatcher)
            .def_readonly("loader", &Tasklet::loader)
            .def_readonly("window", &Tasklet::window)
        ]
      ];
    }

    void LostApplicationTouchEvent(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<TouchEvent, Event, lost::shared_ptr<TouchEvent> >("TouchEvent")
            .def(constructor<std::string>()) 
            .def("size", &TouchEvent::size) 
            .def_readwrite("touches", &TouchEvent::touches, return_stl_iterator)
            .scope
            [
              def("cast", &lost::lua::cast<TouchEvent>),

              class_<TouchEvent::Touch, lost::shared_ptr<TouchEvent::Touch> >("Touch")
                .def(constructor<>()) 
                .def_readwrite("location", &TouchEvent::Touch::location)
                .def_readwrite("tapCount", &TouchEvent::Touch::tapCount)
                .def_readwrite("timeStamp", &TouchEvent::Touch::timeStamp)
            ]
        ]
      ];
      globals(state)["lost"]["application"]["TouchEvent"]["TOUCHES_BEGAN"] = TouchEvent::TOUCHES_BEGAN();
      globals(state)["lost"]["application"]["TouchEvent"]["TOUCHES_MOVED"] = TouchEvent::TOUCHES_MOVED();
      globals(state)["lost"]["application"]["TouchEvent"]["TOUCHES_ENDED"] = TouchEvent::TOUCHES_ENDED();
      globals(state)["lost"]["application"]["TouchEvent"]["TOUCHES_CANCELLED"] = TouchEvent::TOUCHES_CANCELLED();
    }

    void LostApplicationWindow(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<Window, lost::shared_ptr<Window> >("Window")
            .def("open", &Window::open)
            .def("close", &Window::close)
            .def_readonly("context", &Window::context)
            .def_readonly("params", &Window::params)
            .scope
            [
              def("create", &Window::create)
            ],

          class_<WindowParams, lost::shared_ptr<WindowParams> >("WindowParams")
            .def(constructor<const std::string&, const lost::math::Rect&>())
            .def_readwrite("caption", &WindowParams::caption)
            .def_readwrite("rect", &WindowParams::rect)
        ]
      ];
    }

    void LostApplication(lua_State* state)
    {
      LostApplicationApplication(state);
      LostApplicationAccelerometerEvent(state);
      LostApplicationApplicationEvent(state);
      LostApplicationInputEvent(state);
      LostApplicationKeyEvent(state);
      LostApplicationMouseEvent(state);
      LostApplicationResizeEvent(state);
      LostApplicationTasklet(state);
      LostApplicationTouchEvent(state);
      LostApplicationWindow(state);
    }
    
  }
}
