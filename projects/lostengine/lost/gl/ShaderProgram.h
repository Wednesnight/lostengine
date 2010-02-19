#ifndef LOST_GL_SHADERPROGRAM_H
#define LOST_GL_SHADERPROGRAM_H

#include <map>
#include <list>
#include "lost/gl/Shader.h"
#include "lost/common/Color.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec4.h"
#include "lost/platform/shared_ptr.h"

namespace lost
{
namespace gl
{
struct ShaderProgram;
typedef lost::shared_ptr<ShaderProgram> ShaderProgramPtr;

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

    Parameter();
    Parameter(const std::string& inName,
              GLint inIndex,
              GLenum inGlType,
              GLint inSize,
              Parameter::Type inParamType);

    void operator=(float v);
    void operator=(const lost::common::Color& inCol);
    void operator=(const lost::math::Vec2& vec);
    void operator=(const lost::math::Vec3& vec);
    void operator=(const lost::math::Vec4& vec);
    void operator=(GLint v);

    void setInt(GLint inVal);
    void setFloat(float inVal);
    void set(const lost::common::Color& inCol);
    void set(const lost::math::Vec4& vec);
    void set(const lost::math::Vec2& inVec);
    void set(const lost::math::Vec3& inVec);
  };

  typedef std::map<std::string, ShaderProgram::Parameter> ParameterMap;
  typedef std::list<lost::shared_ptr<Shader> >           ShaderList;

  ShaderProgram();
  virtual ~ShaderProgram();
  Parameter& param(const std::string& inName);
  Parameter& operator[](const std::string& inName);
  void attach(lost::shared_ptr<Shader> inShader);
  // detaches shaders and clears all internal references as well as param map
  void detachAllShaders();
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

  ParameterMap parameterMap();

private:
  void buildParamMap();
  void addAttributes();
  void addUniforms();

  GLuint        program;
  ParameterMap  name2param;
  ShaderList    shaders;
};
}
}

#endif
