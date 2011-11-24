#include "lost/lua/lua.h"
#include "lost/lua/bindings/LostBox2D.h"
#include "lost/box2d/DebugDraw.h"
#include "lost/rg/Node.h"
#include "lost/resource/Loader.h"

#include <slub/slub.h>

using namespace lost;
using namespace lost::box2d;

namespace lost
{
  namespace lua
  {

    void LostBox2D(lua_State* state)
    {
      slub::package(state, "lost")
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
