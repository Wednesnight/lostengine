#include "lost/gl/VertexAttribute.h"

namespace lost
{
namespace gl
{

VertexAttribute::VertexAttribute()
{
  index = 0;
  glType = 0;
  size = 0;
  location = 0;
}

VertexAttribute::VertexAttribute(const string& inName, GLint inIndex, GLenum inGlType, GLint inSize, GLint loc)
: name(inName), index(inIndex), glType(inGlType), size(inSize), location(loc)
{
}

VertexAttribute::~VertexAttribute()
{
}

}
}