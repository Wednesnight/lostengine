#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/guiro/View.h"
#include "lost/common/Logger.h"

#include "lost/lua/bindings/LostGuiroView.h"

using namespace luabind;
using namespace lost::event;
using namespace lost::guiro;

struct ViewLuaWrapper : View, luabind::wrap_base
{
  ViewLuaWrapper()
  : View()
  {
    DOUT("ViewLuaWrapper::ViewLuaWrapper()");
  }
  
  ~ViewLuaWrapper()
  {
    DOUT("ViewLuaWrapper::~ViewLuaWrapper()");
  }
  
  virtual void render()
  {
    DOUT("ViewLuaWrapper::render()");
    call<void>("render");
  }
  
  static void renderBase(View* base)
  {
    DOUT("ViewLuaWrapper::renderBase()");
    return base->View::render();
  }
};

namespace lost
{
  namespace lua
  {

    void appendChild(View* parent, View* child)
    {
      DOUT("appendChild()");
      parent->appendChild(child->shared_from_this());
    }
    
    // dummy function for lua type validation
    bool isView(View* view)
    {
      return true;
    }
    
    void LostGuiroView(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("guiro")
        [
          class_<View, ViewLuaWrapper, EventDispatcher, boost::shared_ptr<View> >("View")
            .def(constructor<>())
            .def("appendChild", (void(*)(View*, View*))&appendChild)
            .def("removeChild", &View::removeChild)
            .def("validateChild", &View::validateChild)
            .def("render", &View::render, &ViewLuaWrapper::renderBase)
            .def_readwrite("id", &View::id)
            .property("isView", &isView)
        ]
      ];
    }
  }
}
