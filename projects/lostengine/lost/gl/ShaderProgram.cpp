#include "lost/gl/ShaderProgram.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"

namespace lost
{
namespace gl
{

ShaderProgram::ShaderProgram()
{
  program = glCreateProgram();GLDEBUG_THROW;
}

ShaderProgram::~ShaderProgram()
{
  glDeleteProgram(program);
}

Uniform& ShaderProgram::uniform(const std::string& inName)
{
  ShaderProgram::UniformMap::iterator pos = name2uniform.find(inName);
  if(pos != name2uniform.end())
  {
    return pos->second;
  }
  else
  {
    throw std::runtime_error("couldn't find uniform with name: '"+inName+"'");
  }
}

Uniform& ShaderProgram::operator[](const std::string& inName)
{
  return uniform(inName);
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
  name2uniform.clear();
}

void ShaderProgram::link()
{
  glLinkProgram(program);GLDEBUG_THROW;
  name2uniform.clear();
}

bool ShaderProgram::linked()
{
  return param(GL_LINK_STATUS) == 0 ? false : true;
}

void ShaderProgram::enable()
{
  glUseProgram(program);GLDEBUG_THROW;
  if(name2uniform.size() == 0)
    buildUniformMap();
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

GLenum ShaderProgram::numericalType(const std::string& inName) { return uniform(inName).glType; } 

void ShaderProgram::setInt(const std::string& inName, GLint inVal) {uniform(inName).setInt(inVal);}
void ShaderProgram::setFloat(const std::string& inName, float inVal) {uniform(inName).setFloat(inVal);}
void ShaderProgram::set(const std::string& inName, const lost::common::Color& inVal) {uniform(inName).set(inVal);}
void ShaderProgram::set(const std::string& inName, const lost::math::Vec4& inVal) {uniform(inName).set(inVal);}
void ShaderProgram::set(const std::string& inName, const lost::math::Vec2& inVal) {uniform(inName).set(inVal);}
void ShaderProgram::set(const std::string& inName, const lost::math::Vec3& inVal) {uniform(inName).set(inVal);}
void ShaderProgram::set(const std::string& inName, const math::Matrix& inVal) {uniform(inName).set(inVal);}

bool ShaderProgram::hasParam(const std::string& name)
{
  return (name2uniform.find(name) != name2uniform.end());
}


ShaderProgram::UniformMap& ShaderProgram::uniformMap()
{
  return name2uniform;
}

void ShaderProgram::buildUniformMap()
{
  name2uniform.clear();
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
    Uniform uniform(buffer.get(), i, type, size, loc);
    name2uniform[uniform.name] = uniform;
//    DOUT(i << " : " << std::string(buffer.get(), buffer.get()+writtenBytes) << " size:"<<size << " type:"<<lost::gl::utils::enum2string(type));
  }
}


}
}