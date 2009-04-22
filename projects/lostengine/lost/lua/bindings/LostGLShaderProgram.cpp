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
          def("loadShader", loadShader),
          class_<ShaderProgram, ShaderProgramPtr >("ShaderProgram")
            .def(constructor<>())
            .def("enable", &ShaderProgram::enable)
            .def("disable", &ShaderProgram::disable)
            .def("log", &ShaderProgram::log)
            .def("validate", &ShaderProgram::validate)
            .def("validated", &ShaderProgram::validated)
            .def("setInt", (void(ShaderProgram::*)(const std::string& inName, GLint inVal)) &ShaderProgram::setInt)
            .def("setFloat", (void(ShaderProgram::*)(const std::string& inName, float inVal)) &ShaderProgram::setFloat)
            .def("set", (void(ShaderProgram::*)(const std::string& inName, const lost::common::Color& inVal)) &ShaderProgram::set)
            .def("set", (void(ShaderProgram::*)(const std::string& inName, const lost::math::Vec4& inVal))  &ShaderProgram::set)
            .def("set", (void(ShaderProgram::*)(const std::string& inName, const lost::math::Vec2& inVal)) &ShaderProgram::set)
            .def("set", (void(ShaderProgram::*)(const std::string& inName, const lost::math::Vec3& inVal)) &ShaderProgram::set)            
        ]
      ];
    }    
  }
}
