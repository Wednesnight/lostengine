#ifndef _OBJ_FACE_H
#define _OBJ_FACE_H

#include <vector>
#include <objlib/vertex.h>

namespace obj
{

/** a face can have an arbitrary number of vertices
 */
struct face
{
    face() {}
    
    void clear() { vertices.clear(); }
    
    std::vector<obj::vertex> vertices;
};

inline std::ostream& operator << (std::ostream& stream, const face& f)
{
    for(unsigned long i = 0; i<f.vertices.size(); ++i)
    {
        stream << f.vertices[i];
    }
    return stream;
} 

}

#endif