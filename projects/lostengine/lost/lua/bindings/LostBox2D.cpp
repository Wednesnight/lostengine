/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
            .constructor<resource::LoaderPtr, lua_State*>()
            .method("beginDraw",&DebugDraw::beginDraw)
            .method("endDraw",&DebugDraw::endDraw)
            .field("rootNode",&DebugDraw::rootNode);
    }

  }
}
