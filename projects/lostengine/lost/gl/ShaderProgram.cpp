#include "lost/gl/ShaderProgram.h"

#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"

namespace lost
{
namespace gl
{

ShaderProgram::Parameter::Parameter()
{
  glType = 0;
  size = 0;
  paramType = UNDEFINED;
}

ShaderProgram::Parameter::Parameter(const std::string& inName, GLint inIndex, GLenum inGlType, GLint inSize, Parameter::Type inParamType, GLint loc)
: name(inName), index(inIndex), glType(inGlType), size(inSize), paramType(inParamType), location(loc)
{
}

void ShaderProgram::Parameter::operator=(float v) { setFloat(v); }
void ShaderProgram::Parameter::operator=(const lost::common::Color& inCol) { set(inCol); }
void ShaderProgram::Parameter::operator=(const lost::math::Vec2& vec) { set(vec); }
void ShaderProgram::Parameter::operator=(const lost::math::Vec3& vec) { set(vec); }
void ShaderProgram::Parameter::operator=(const lost::math::Vec4& vec) { set(vec); }
void ShaderProgram::Parameter::operator=(GLint v) { setInt(v); }

void ShaderProgram::Parameter::setInt(GLint inVal)
{
  glUniform1i(location, inVal);GLDEBUG_THROW;
}

void ShaderProgram::Parameter::setFloat(float inVal)
{
  glUniform1f(location, inVal);GLDEBUG_THROW;
}

void ShaderProgram::Parameter::set(const lost::common::Color& inCol)
{
    switch(glType)
    {
      case GL_FLOAT_VEC3: glUniform3f(location, inCol.r(), inCol.g(), inCol.b());GLDEBUG_THROW;break;
      case GL_FLOAT_VEC4: glUniform4f(location, inCol.r(), inCol.g(), inCol.b(), inCol.a());GLDEBUG_THROW;break;
      default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Color'");
    }
}

void ShaderProgram::Parameter::set(const lost::math::Vec4& vec)
{
    switch(glType)
    {
      case GL_FLOAT_VEC3: glUniform3f(location, vec.x, vec.y, vec.z);GLDEBUG_THROW;break;
      case GL_FLOAT_VEC4: glUniform4f(location, vec.x, vec.y, vec.z, vec.w);GLDEBUG_THROW;break;
      default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Vec4'");
    }
}

void ShaderProgram::Parameter::set(const lost::math::Vec2& inVec)
{
    switch(glType)
    {
      case GL_FLOAT_VEC2: glUniform2f(location, inVec.x, inVec.y);GLDEBUG_THROW;break;
      default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Vec2'");
    }
}

void ShaderProgram::Parameter::set(const lost::math::Vec3& inVec)
{
    switch(glType)
    {
      case GL_FLOAT_VEC3: glUniform3f(location, inVec.x, inVec.y, inVec.z);GLDEBUG_THROW;break;
      default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Vec3'");
    }
}

void ShaderProgram::Parameter::set(const math::Matrix& mat)
{
  switch(glType)
  {
    case GL_FLOAT_MAT4: glUniformMatrix4fv(location, 1, GL_FALSE, mat.m);GLDEBUG_THROW;break;
    default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Matrix'");
  }
}


ShaderProgram::ShaderProgram()
{
  program = glCreateProgram();GLDEBUG_THROW;
}

ShaderProgram::~ShaderProgram()
{
  glDeleteProgram(program);
}

ShaderProgram::Parameter& ShaderProgram::param(const std::string& inName)
{
  ParameterMap::iterator pos = name2param.find(inName);
  if(pos != name2param.end())
  {
    return pos->second;
  }
  else
  {
    throw std::runtime_error("couldn't find parameter with name: '"+inName+"'");
  }
}

ShaderProgram::Parameter& ShaderProgram::operator[](const std::string& inName)
{
  return param(inName);
}

void ShaderProgram::attach(lost::shared_ptr<Shader> inShader)
{
  glAttachShader(program, inShader->shader);GLDEBUG_THROW;
  shaders.push_back(inShader);
}

// detaches shaders and clears all internal references as well as param map
void ShaderProgram::detachAllShaders()
{
  for(ShaderList::iterator i=shaders.begin(); i!=shaders.end(); ++i)
  {
    glDetachShader(program, (*i)->shader);GLDEBUG_THROW;
  }
  shaders.clear();
  name2param.clear();
}

void ShaderProgram::link()
{
  glLinkProgram(program);GLDEBUG_THROW;
  name2param.clear();
}

bool ShaderProgram::linked()
{
  return param(GL_LINK_STATUS) == 0 ? false : true;
}

void ShaderProgram::enable()
{
  glUseProgram(program);GLDEBUG_THROW;
  if(name2param.size() == 0)
    buildParamMap();
}

void ShaderProgram::disable()
{
  glUseProgram(0); // don't check for error here because calling it with 0 always results in an error, which is perfectly ok
}

// returns information about the current program state
GLint ShaderProgram::param(GLenum paramName)
{
  GLint result;
  glGetProgramiv(program, paramName, &result);GLDEBUG_THROW;
  return result;
}

std::string ShaderProgram::log()
{
  GLint size = param(GL_INFO_LOG_LENGTH);
  std::string result;
  if(size > 0)
  {
    shared_array<char> buffer(new char[size]);
    GLsizei length = 0;
    glGetProgramInfoLog(program, size, &length, buffer.get());GLDEBUG_THROW;
    result = std::string(buffer.get(), buffer.get()+length);
  }
  return result;
}

void ShaderProgram::validate()
{
  glValidateProgram(program);GLDEBUG_THROW;
}

bool ShaderProgram::validated()
{
  return (param(GL_VALIDATE_STATUS) == GL_TRUE) ? true : false;
}

GLenum ShaderProgram::numericalType(const std::string& inName) { return param(inName).glType; } 

void ShaderProgram::setInt(const std::string& inName, GLint inVal) {param(inName).setInt(inVal);}
void ShaderProgram::setFloat(const std::string& inName, float inVal) {param(inName).setFloat(inVal);}
void ShaderProgram::set(const std::string& inName, const lost::common::Color& inVal) {param(inName).set(inVal);}
void ShaderProgram::set(const std::string& inName, const lost::math::Vec4& inVal) {param(inName).set(inVal);}
void ShaderProgram::set(const std::string& inName, const lost::math::Vec2& inVal) {param(inName).set(inVal);}
void ShaderProgram::set(const std::string& inName, const lost::math::Vec3& inVal) {param(inName).set(inVal);}
void ShaderProgram::set(const std::string& inName, const math::Matrix& inVal) {param(inName).set(inVal);}

bool ShaderProgram::hasParam(const std::string& name)
{
  return (name2param.find(name) != name2param.end());
}


ShaderProgram::ParameterMap ShaderProgram::parameterMap()
{
  return name2param;
}

void ShaderProgram::buildParamMap()
{
  name2param.clear();
  addUniforms();
}

void ShaderProgram::addUniforms()
{
  GLint numAttributes = param(GL_ACTIVE_UNIFORMS);
  int bufferSize = param(GL_ACTIVE_UNIFORM_MAX_LENGTH);
  shared_array<char> buffer(new char[bufferSize]);
  GLint writtenBytes = 0;
  GLenum type;
  GLint size;

  for(GLint i=0; i<numAttributes; ++i)
  {
    glGetActiveUniform(program, i, bufferSize, &writtenBytes, &size, &type, buffer.get());
    GLint loc = glGetUniformLocation(program, buffer.get());
    Parameter param(buffer.get(), i, type, size, Parameter::UNIFORM, loc);
    name2param[param.name] = param;
//    DOUT(i << " : " << std::string(buffer.get(), buffer.get()+writtenBytes) << " size:"<<size << " type:"<<lost::gl::utils::enum2string(type));
  }
}


}
}