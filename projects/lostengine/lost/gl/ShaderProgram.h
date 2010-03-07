#ifndef LOST_GL_SHADERPROGRAM_H
#define LOST_GL_SHADERPROGRAM_H

#include <map>
#include <list>
#include "lost/gl/Shader.h"
#include "lost/platform/shared_ptr.h"
#include "lost/gl/Uniform.h"
#include "lost/gl/VertexAttribute.h"

namespace lost
{
namespace gl
{
struct ShaderProgram;
typedef lost::shared_ptr<ShaderProgram> ShaderProgramPtr;

/** NOTE: in order to minimise side effects of functions, you manually have to call 
 *  buildUniformMap() and buildVertexAttributeMap() after you compiled, linked and enabled the shader.
 */
struct ShaderProgram
{
public:
typedef std::map<std::string, Uniform>          UniformMap;
typedef std::map<std::string, VertexAttribute>  VertexAttributeMap;
typedef std::list<ShaderPtr>                    ShaderList;

  ShaderProgram();
  virtual ~ShaderProgram();
  Uniform& uniform(const std::string& inName);
  Uniform& operator[](const std::string& inName);
  void attach(const ShaderPtr& inShader);
  void detachAllShaders(); // detaches all shaders, throwing awa all internal references to shader objects
  void link();
  bool linked();
  void enable();
  void disable();
  // returns information about the current program state
  GLint param(GLenum paramName);
  std::string log();
  void validate();
  bool validated();
  GLenum numericalType(const std::string& inName);

  void setInt(const std::string& inName, GLint inVal);
  void setFloat(const std::string& inName, float inVal);
  void set(const std::string& inName, const lost::common::Color& inVal);
  void set(const std::string& inName, const lost::math::Vec4& inVal);
  void set(const std::string& inName, const lost::math::Vec2& inVal);
  void set(const std::string& inName, const lost::math::Vec3& inVal);
  void set(const std::string& inName, const math::Matrix& inVal);
  bool hasUniform(const std::string& name);

  UniformMap& uniformMap();

  // uniform and vertexAttribute maps need to be rebuild each time you link the shader program
  void buildUniformMap();
  void buildVertexAttributeMap();

  GLuint              program;
  UniformMap          name2uniform;
  VertexAttributeMap  name2vertexAttribute;
  ShaderList          shaders;
};
}
}

#endif
