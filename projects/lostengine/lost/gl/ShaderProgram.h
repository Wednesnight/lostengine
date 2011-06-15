#ifndef LOST_GL_SHADERPROGRAM_H
#define LOST_GL_SHADERPROGRAM_H

#include "lost/gl/Shader.h"
#include "lost/gl/Uniform.h"
#include "lost/gl/VertexAttribute.h"
#include "lost/gl/forward.h"

namespace lost
{
namespace gl
{

/** NOTE: in order to minimise side effects of functions, you manually have to call 
 *  buildUniformMap() and buildVertexAttributeMap() after you compiled, linked and enabled the shader.
 */
struct ShaderProgram
{
public:
typedef map<string, Uniform>          UniformMap;
typedef map<string, VertexAttribute>  VertexAttributeMap;
typedef list<ShaderPtr>                    ShaderList;

  ShaderProgram();
  virtual ~ShaderProgram();
  static ShaderProgramPtr create() { return ShaderProgramPtr(new ShaderProgram); }

  Uniform& uniform(const string& inName);
  Uniform& operator[](const string& inName);
  void attach(ShaderPtr& inShader);
  void detachAllShaders(); // detaches all shaders, throwing awa all internal references to shader objects
  void link();
  bool linked();
  void enable();
  void disable();
  // returns information about the current program state
  GLint param(GLenum paramName);
  string log();
  void validate();
  bool validated();
  GLenum numericalType(const string& inName);

  void setInt(const string& inName, GLint inVal);
  void setFloat(const string& inName, float inVal);
  void setBool(const string& inName, bool inVal);
  void set(const string& inName, const lost::common::Color& inVal);
  void set(const string& inName, const lost::math::Vec4& inVal);
  void set(const string& inName, const lost::math::Vec2& inVal);
  void set(const string& inName, const lost::math::Vec3& inVal);
  void set(const string& inName, const math::Matrix& inVal);
  bool hasUniform(const string& name);
  bool hasAttribute(const string& name);

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
