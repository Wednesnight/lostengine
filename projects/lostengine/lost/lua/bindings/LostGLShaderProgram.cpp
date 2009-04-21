#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include "lost/lua/lua.h"
#include "lost/gl/ShaderProgram.h"
#include "lost/lua/bindings/LostGLShaderProgram.h"
#include "lost/gl/ShaderHelper.h"

using namespace luabind;
using namespace lost::gl;

namespace lost
{
  namespace lua
  {
    void LostGLShaderProgram(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<ShaderProgram, ShaderProgramPtr >("ShaderProgram")
            .def(constructor<>())
          .scope
          [
            def("loadShader", loadShader)
          ]
        ]
      ];
    }    
  }
}
