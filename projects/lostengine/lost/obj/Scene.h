#ifndef _OBJ_SCENE_H
#define _OBJ_SCENE_H

#include <vector>
#include "lobobase/math/vec4.h"
#include "objlib/face.h"

namespace obj
{

struct scene
{
    scene() {};
    
    void clear()
    {
        vertices.clear();
        normals.clear();
        texcoords.clear();
        faces.clear();
    }
    
    std::vector<lobobase::math::vec4> vertices;
    std::vector<lobobase::math::vec4> normals;
    std::vector<lobobase::math::vec4> texcoords;    
    
    std::vector<obj::face> faces;
};

inline std::ostream& operator << (std::ostream& stream, const scene& s)
{
    stream << "vertices\n";
    for(unsigned long i = 0; i<s.vertices.size(); ++i) stream << s.vertices[i];
    stream << "\nnormals\n";
    for(unsigned long i = 0; i<s.normals.size(); ++i) stream << s.normals[i];
    stream << "\ntexcoords\n";
    for(unsigned long i = 0; i<s.texcoords.size(); ++i) stream << s.texcoords[i];
    stream << "\nfaces\n";
    for(unsigned long i = 0; i<s.faces.size(); ++i) stream << s.faces[i];

    return stream;
} 

}

#endif