#include "lost/gl/Uniform.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"

namespace lost
{
namespace gl
{

Uniform::Uniform()
{
  index = 0;
  glType = 0;
  size = 0;
  location = 0;
}

Uniform::Uniform(const std::string& inName, GLint inIndex, GLenum inGlType, GLint inSize, GLint loc)
: name(inName), index(inIndex), glType(inGlType), size(inSize), location(loc)
{
}

void Uniform::operator=(float v) { setFloat(v); }
void Uniform::operator=(const lost::common::Color& inCol) { set(inCol); }
void Uniform::operator=(const lost::math::Vec2& vec) { set(vec); }
void Uniform::operator=(const lost::math::Vec3& vec) { set(vec); }
void Uniform::operator=(const lost::math::Vec4& vec) { set(vec); }
void Uniform::operator=(GLint v) { setInt(v); }

void Uniform::setInt(GLint inVal)
{
  glUniform1i(location, inVal);GLDEBUG_THROW;
}

void Uniform::setFloat(float inVal)
{
  glUniform1f(location, inVal);GLDEBUG_THROW;
}

void Uniform::setBool(bool inVal)
{
  glUniform1f(location, inVal);GLDEBUG_THROW;
}

void Uniform::set(const lost::common::Color& inCol)
{
    switch(glType)
    {
      case GL_FLOAT_VEC3: glUniform3f(location, inCol.r(), inCol.g(), inCol.b());GLDEBUG_THROW;break;
      case GL_FLOAT_VEC4: glUniform4f(location, inCol.r(), inCol.g(), inCol.b(), inCol.a());GLDEBUG_THROW;break;
      default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Color'");
    }
}

void Uniform::set(const lost::math::Vec4& vec)
{
    switch(glType)
    {
      case GL_FLOAT_VEC3: glUniform3f(location, vec.x, vec.y, vec.z);GLDEBUG_THROW;break;
      case GL_FLOAT_VEC4: glUniform4f(location, vec.x, vec.y, vec.z, vec.w);GLDEBUG_THROW;break;
      default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Vec4'");
    }
}

void Uniform::set(const lost::math::Vec2& inVec)
{
    switch(glType)
    {
      case GL_FLOAT_VEC2: glUniform2f(location, inVec.x, inVec.y);GLDEBUG_THROW;break;
      default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Vec2'");
    }
}

void Uniform::set(const lost::math::Vec3& inVec)
{
    switch(glType)
    {
      case GL_FLOAT_VEC3: glUniform3f(location, inVec.x, inVec.y, inVec.z);GLDEBUG_THROW;break;
      default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Vec3'");
    }
}

void Uniform::set(const math::Matrix& mat)
{
  switch(glType)
  {
    case GL_FLOAT_MAT4: glUniformMatrix4fv(location, 1, GL_FALSE, mat.m);GLDEBUG_THROW;break;
    default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Matrix'");
  }
}

}
}