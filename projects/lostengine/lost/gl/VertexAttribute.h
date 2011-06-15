#ifndef LOST_GL_VERTEXATTRIBUTE_H
#define LOST_GL_VERTEXATTRIBUTE_H

#include "lost/gl/gltypes.h"

namespace lost
{
namespace gl
{

struct VertexAttribute
{
  string     name;
  GLint           index;
  GLenum          glType;     // numerical data type (int, float, bool etc.)
  GLint           size;       // size of array
  GLint           location;

  VertexAttribute();
  VertexAttribute(const string& inName,
          GLint inIndex,
          GLenum inGlType,
          GLint inSize,
          GLint loc);

  ~VertexAttribute();
  
  
};

}
}

#endif