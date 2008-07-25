#ifndef _OBJ_VERTEX_H
#define _OBJ_VERTEX_H

#include <iosfwd>

namespace lost
{
  namespace model
  {

    struct Vertex
    {
      float x;
      float y;
      float z;
      float w;
      
      Vertex()
      {
        clear();
      }
      
      Vertex(float inX, float inY, float inZ, float inW)
      : x(inX),
        y(inY),
        z(inZ),
        w(inW)
      {
      }
      
      void clear()
      {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
      }
    };

    inline std::ostream& operator << (std::ostream& stream, const Vertex& v)
    {
        stream << "(" << v.x << "/" << v.y << "/" << v.z << "/" << v.w << ")";
        return stream;
    }

  }
}

#endif
