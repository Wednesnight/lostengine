#include "lost/lua/lua.h"
#include "lost/lua/bindings/LostBox2D.h"
#include "lost/box2d/DebugDraw.h"
#include "lost/rg/Node.h"
#include "lost/resource/Loader.h"

#include <slub/slub.h>

using namespace lost;
using namespace lost::box2d;
using namespace slub;

namespace slub {

  template<>
  struct converter<resource::LoaderPtr> {
    
    static bool check(lua_State* L, int index) {
      return true;
    }
    
    static resource::LoaderPtr get(lua_State* L, int index) {
      return luabind::object_cast<resource::LoaderPtr>(luabind::object(luabind::from_stack(L, index)));
    }
    
    static int push(lua_State* L, resource::LoaderPtr loader) {
      luabind::object o = luabind::object(L, loader);
      o.push(L);
      return 1;
    }
    
  };  

  template<>
  struct converter<rg::NodePtr> {
    
    static bool check(lua_State* L, int index) {
      return true;
    }
    
    static rg::NodePtr get(lua_State* L, int index) {
      return luabind::object_cast<rg::NodePtr>(luabind::object(luabind::from_stack(L, index)));
    }
    
    static int push(lua_State* L, rg::NodePtr node) {
      luabind::object o = luabind::object(L, node);
      o.push(L);
      return 1;
    }
    
  };  
}

namespace lost
{
  namespace lua
  {

    void LostBox2D(lua_State* state)
    {
      package(state, "lost")
        .package("box2d")
          .clazz<DebugDraw>("DebugDraw")
            .extends<b2DebugDraw>()
            .constructor<resource::LoaderPtr>()
            .method("beginDraw",&DebugDraw::beginDraw)
            .method("endDraw",&DebugDraw::endDraw)
            .field("rootNode",&DebugDraw::rootNode);
    }

  }
}
