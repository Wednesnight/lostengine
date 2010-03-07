#ifndef LOST_GL_VERTEXATTRIBUTE_H
#define LOST_GL_VERTEXATTRIBUTE_H

#include "lost/gl/gltypes.h"
#include <string>

namespace lost
{
namespace gl
{

struct VertexAttribute
{
  std::string     name;
  GLint           index;
  GLenum          glType;     // numerical data type (int, float, bool etc.)
  GLint           size;       // size of array
  GLint           location;

  VertexAttribute();
  VertexAttribute(const std::string& inName,
          GLint inIndex,
          GLenum inGlType,
          GLint inSize,
          GLint loc);

  ~VertexAttribute();
  
  
};

}
}

#endif