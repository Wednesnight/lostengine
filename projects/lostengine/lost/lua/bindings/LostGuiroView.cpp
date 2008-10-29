#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/guiro/View.h"

#include "lost/lua/bindings/LostGuiroView.h"

using namespace luabind;
using namespace lost::event;
using namespace lost::guiro;

struct ViewLuaWrapper : View, luabind::wrap_base
{
  ViewLuaWrapper()
  : View()
  {
  }
  
  virtual void render()
  {
    call<void>("render");
  }
  
  static void renderBase(View* base)
  {
    return base->View::render();
  }
};

namespace lost
{
  namespace lua
  {
    void appendChild(object parent, object child)
    {
      if (luabind::type(child) == LUA_TNIL) { throw std::runtime_error("can't add NIL child"); }
      View* parentView = object_cast<View*>(parent);
      View* childView  = object_cast<View*>(child);
      parentView->appendChild(boost::shared_ptr<View>(childView));
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
          class_<View, EventDispatcher, boost::shared_ptr<View>, ViewLuaWrapper>("View")
            .def(constructor<>())
            .def("appendChild", &appendChild)
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
