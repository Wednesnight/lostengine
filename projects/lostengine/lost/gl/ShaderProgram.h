#ifndef LOST_GL_SHADERPROGRAM_H
#define LOST_GL_SHADERPROGRAM_H

#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/gl/Shader.h"
#include "lost/common/Color.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec4.h"

namespace lost
{
namespace gl
{
struct ShaderProgram;
typedef boost::shared_ptr<ShaderProgram> ShaderProgramPtr;

struct ShaderProgram
{
public:
  struct Parameter
  {
    enum Type
    {
      UNDEFINED = 0,
      ATTRIBUTE = 1,
      UNIFORM = 2
    };

    std::string     name;
    GLint           index;
    GLenum          glType;     // numerical data type (int, float, bool etc.)
    GLint           size;       // size of array
    Parameter::Type paramType;  // shader program parameter type, i.e. attribute or uniform

    Parameter()
    {
      glType = 0;
      size = 0;
      paramType = UNDEFINED;
    }

    Parameter(const std::string& inName, GLint inIndex, GLenum inGlType, GLint inSize, Parameter::Type inParamType)
    : name(inName), index(inIndex), glType(inGlType), size(inSize), paramType(inParamType)
    {}

    void operator=(float v) { set(v); }
    void operator=(const lost::common::Color& inCol) { set(inCol); }
    void operator=(const lost::math::Vec2& vec) { set(vec); }
    void operator=(const lost::math::Vec3& vec) { set(vec); }
    void operator=(const lost::math::Vec4& vec) { set(vec); }
    void operator=(GLuint v) { set(v); }

    void set(GLuint inVal)
    {
      if(paramType == UNIFORM)
      {  glUniform1i(index, inVal);GLDEBUG_THROW;}
      else
      {  throw std::runtime_error("paramType undefined for: '"+name+"'");}
    }

    void set(float inVal)
    {
      if(paramType == UNIFORM)
      {  glUniform1f(index, inVal);GLDEBUG_THROW;}
      else if(paramType == ATTRIBUTE)
      {  glVertexAttrib1f(index, inVal);GLDEBUG_THROW;}
      else
      {  throw std::runtime_error("paramType undefined for: '"+name+"'");}
    }

    void set(const lost::common::Color& inCol)
    {
      if(paramType == UNIFORM)
      {
        switch(glType)
        {
          case GL_FLOAT_VEC3: glUniform3f(index, inCol.r(), inCol.g(), inCol.b());GLDEBUG_THROW;break;
          case GL_FLOAT_VEC4: glUniform4f(index, inCol.r(), inCol.g(), inCol.b(), inCol.a());GLDEBUG_THROW;break;
          default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Color'");
        }
      }
      else if(paramType == ATTRIBUTE)
      {
        switch(glType)
        {
          case GL_FLOAT_VEC3: glVertexAttrib3f(index, inCol.r(), inCol.g(), inCol.b());GLDEBUG_THROW;break;
          case GL_FLOAT_VEC4: glVertexAttrib4f(index, inCol.r(), inCol.g(), inCol.b(), inCol.a());GLDEBUG_THROW;break;
          default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Color'");
        }
      }
      else
        throw std::runtime_error("paramType undefined for: '"+name+"'");
    }

    void set(const lost::math::Vec4& vec)
    {
      if(paramType == UNIFORM)
      {
        switch(glType)
        {
          case GL_FLOAT_VEC3: glUniform3f(index, vec.x, vec.y, vec.z);GLDEBUG_THROW;break;
          case GL_FLOAT_VEC4: glUniform4f(index, vec.x, vec.y, vec.z, vec.w);GLDEBUG_THROW;break;
          default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Vec4'");
        }
      }
      else if(paramType == ATTRIBUTE)
      {
        switch(glType)
        {
          case GL_FLOAT_VEC3: glVertexAttrib3f(index, vec.x, vec.y, vec.z);GLDEBUG_THROW;break;
          case GL_FLOAT_VEC4: glVertexAttrib4f(index, vec.x, vec.y, vec.z, vec.w);GLDEBUG_THROW;break;
          default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Vec4'");
        }
      }
      else
        throw std::runtime_error("paramType undefined for: '"+name+"'");
    }

    void set(const lost::math::Vec2& inVec)
    {
      if(paramType == UNIFORM)
      {
        switch(glType)
        {
          case GL_FLOAT_VEC2: glUniform2f(index, inVec.x, inVec.y);GLDEBUG_THROW;break;
          default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Vec2'");
        }
      }
      else if(paramType == ATTRIBUTE)
      {
        switch(glType)
        {
          case GL_FLOAT_VEC3: glVertexAttrib2f(index, inVec.x, inVec.y);GLDEBUG_THROW;break;
          default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Vec2'");
        }
      }
      else
        throw std::runtime_error("paramType undefined for: '"+name+"'");
    }

    void set(const lost::math::Vec3& inVec)
    {
      if(paramType == UNIFORM)
      {
        switch(glType)
        {
          case GL_FLOAT_VEC3: glUniform3f(index, inVec.x, inVec.y, inVec.z);GLDEBUG_THROW;break;
          default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Vec3'");
        }
      }
      else if(paramType == ATTRIBUTE)
      {
        switch(glType)
        {
          case GL_FLOAT_VEC3: glVertexAttrib3f(index, inVec.x, inVec.y, inVec.z);GLDEBUG_THROW;break;
          default: throw std::runtime_error("type mismatch for: '"+name+"' required: "+lost::gl::utils::enum2string(glType)+" was 'Vec3'");
        }
      }
      else
        throw std::runtime_error("paramType undefined for: '"+name+"'");
    }

  };

  typedef std::map<std::string, ShaderProgram::Parameter> ParameterMap;
  typedef std::list<boost::shared_ptr<Shader> >           ShaderList;

  ShaderProgram()
  {
    program = glCreateProgram();GLDEBUG_THROW;
  }

  virtual ~ShaderProgram()
  {
    glDeleteProgram(program);
  }

  Parameter& operator[](const std::string& inName)
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

  void attach(boost::shared_ptr<Shader> inShader)
  {
    glAttachShader(program, inShader->shader);GLDEBUG_THROW;
    shaders.push_back(inShader);
  }

  // detaches shaders and clears all internal references as well as param map
  void detachAllShaders()
  {
    for(ShaderList::iterator i=shaders.begin(); i!=shaders.end(); ++i)
    {
      glDetachShader(program, (*i)->shader);GLDEBUG_THROW;
    }
    shaders.clear();
    name2param.clear();
  }

  void link()
  {
    glLinkProgram(program);GLDEBUG_THROW;
    name2param.clear();
  }

  bool linked()
  {
    return param(GL_LINK_STATUS) == 0 ? false : true;
  }

  void enable()
  {
    glUseProgram(program);GLDEBUG_THROW;
    if(name2param.size() == 0)
      buildParamMap();
  }

  void disable()
  {
    glUseProgram(0); // don't check for error here because calling it with 0 always results in an error, which is perfectly ok
  }

  // returns information about the current program state
  GLint param(GLenum paramName)
  {
    GLint result;
    glGetProgramiv(program, paramName, &result);GLDEBUG_THROW;
    return result;
  }

  std::string log()
  {
    GLint size = param(GL_INFO_LOG_LENGTH);
    boost::scoped_array<char> buffer;
    std::string result;
    if(size > 0)
    {
      boost::scoped_array<char> buffer(new char[size]);
      GLsizei length = 0;
      glGetProgramInfoLog(program, size, &length, buffer.get());GLDEBUG_THROW;
      result = std::string(buffer.get(), buffer.get()+length);
    }
    return result;
  }

  void validate()
  {
    glValidateProgram(program);GLDEBUG_THROW;
  }

  bool validated()
  {
    return (param(GL_VALIDATE_STATUS) == GL_TRUE) ? true : false;
  }

private:
  void buildParamMap()
  {
    name2param.clear();
    addAttributes();
    addUniforms();
  }

  void addAttributes()
  {
    GLint numAttributes = param(GL_ACTIVE_ATTRIBUTES);
    int bufferSize = param(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH);;
    boost::scoped_ptr<char> buffer(new char[bufferSize]);
    GLint writtenBytes = 0;
    GLenum type;
    GLint size;

    for(GLint i=0; i<numAttributes; ++i)
    {
      glGetActiveAttrib(program, i, bufferSize, &writtenBytes, &size, &type, buffer.get());
      Parameter param(buffer.get(), i, type, size, Parameter::ATTRIBUTE);
      name2param[param.name] = param;
      DOUT(i << " : " << std::string(buffer.get(), buffer.get()+writtenBytes) << " size:"<<size << " type:"<<lost::gl::utils::enum2string(type));
    }
  }

  void addUniforms()
  {
    GLint numAttributes = param(GL_ACTIVE_UNIFORMS);
    int bufferSize = param(GL_ACTIVE_UNIFORM_MAX_LENGTH);
    boost::scoped_ptr<char> buffer(new char[bufferSize]);
    GLint writtenBytes = 0;
    GLenum type;
    GLint size;

    for(GLint i=0; i<numAttributes; ++i)
    {
      glGetActiveUniform(program, i, bufferSize, &writtenBytes, &size, &type, buffer.get());
      Parameter param(buffer.get(), i, type, size, Parameter::UNIFORM);
      name2param[param.name] = param;
      DOUT(i << " : " << std::string(buffer.get(), buffer.get()+writtenBytes) << " size:"<<size << " type:"<<lost::gl::utils::enum2string(type));
    }
  }

  GLuint        program;
  ParameterMap  name2param;
  ShaderList    shaders;
};
}
}

#endif
