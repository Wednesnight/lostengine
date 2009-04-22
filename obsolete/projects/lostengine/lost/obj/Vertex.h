#ifndef LOST_OBJ_VERTEX_H
#define LOST_OBJ_VERTEX_H

#include <iosfwd>

namespace lost
{
  namespace obj
  {

    struct Vertex
    {
        Vertex() {};
        
        void clear()
        {
            vertex_index = 0;
            normal_index = 0;
            texcoord_index = 0;
        }
        
        unsigned long vertex_index;
        unsigned long normal_index;
        unsigned long texcoord_index;
    };

    inline std::ostream& operator << (std::ostream& stream, const Vertex& v)
    {
        stream << "(" << v.vertex_index << "/" << v.texcoord_index << "/" << v.normal_index << ")";
        return stream;
    }

  }
}

#endif
