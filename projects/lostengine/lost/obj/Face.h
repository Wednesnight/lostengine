#ifndef _OBJ_FACE_H
#define _OBJ_FACE_H

#include <vector>
#include <lost/obj/Vertex.h>

namespace lost
{
  namespace obj
  {

    /** a face can have an arbitrary number of vertices
     */
    struct Face
    {
        Face() {}
        
        void clear() { vertices.clear(); }
        
        std::vector<Vertex> vertices;
    };

    inline std::ostream& operator << (std::ostream& stream, const Face& f)
    {
        for(unsigned long i = 0; i<f.vertices.size(); ++i)
        {
            stream << f.vertices[i];
        }
        return stream;
    } 

  }
}

#endif
