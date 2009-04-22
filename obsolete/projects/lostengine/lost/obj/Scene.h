#ifndef LOST_OBJ_SCENE_H
#define LOST_OBJ_SCENE_H

#include <vector>
#include "lost/math/Vec4.h"
#include "lost/obj/Face.h"

namespace lost
{
  namespace obj
  {

    struct Scene
    {
        Scene() {};
        
        void clear()
        {
            vertices.clear();
            normals.clear();
            texcoords.clear();
            faces.clear();
        }
        
        std::vector<lost::math::Vec4> vertices;
        std::vector<lost::math::Vec4> normals;
        std::vector<lost::math::Vec4> texcoords;    
        
        std::vector<lost::obj::Face> faces;
    };

    inline std::ostream& operator << (std::ostream& stream, const Scene& s)
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
}

#endif
