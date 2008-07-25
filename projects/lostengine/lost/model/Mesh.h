#ifndef LOST_MODEL_MESH_H
#define LOST_MODEL_MESH_H

#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include "lost/math/Vec3.h"
#include "lost/common/Logger.h"
#include "lost/math/AABB.h"

namespace lost
{
  namespace model
  {
    
    struct Mesh
    {
      unsigned int                          vertexCount;
      boost::shared_array<lost::math::Vec3> vertices;

      lost::math::AABB box;

      Mesh(unsigned int inVertexCount)
      : vertexCount(inVertexCount)
      {
        vertices.reset(new lost::math::Vec3[vertexCount]);
        clear();
      }
      
      ~Mesh() {}

      void clear()
      {
        for (unsigned int idx = 0; idx < vertexCount; ++idx)
        {
          vertices[idx].zero();
        }
      }

      void setVertex(unsigned int inIndex, lost::math::Vec3& inVertex)
      {
        if (inIndex < vertexCount)
        {
          vertices[inIndex] = inVertex;
          box.origin.x = std::min(box.origin.x, inVertex.x);
          box.origin.y = std::min(box.origin.y, inVertex.y);
          box.origin.z = std::min(box.origin.z, inVertex.z);
          box.size.x = std::max(box.size.x + box.origin.x, inVertex.x) - box.origin.x;
          box.size.y = std::max(box.size.y + box.origin.y, inVertex.y) - box.origin.y;
          box.size.z = std::max(box.size.z + box.origin.z, inVertex.z) - box.origin.z;
        }
      }
      
    };
    
    inline std::ostream& operator << (std::ostream& stream, const boost::shared_ptr<Mesh>& m)
    {
      stream << "vertexCount: " << m->vertexCount << std::endl;
      for (unsigned int idx = 0; idx < m->vertexCount; ++idx)
        stream << "vertices[" << idx << "]: " << m->vertices[idx] << std::endl;
      return stream;
    }
    
  }
}

#endif
