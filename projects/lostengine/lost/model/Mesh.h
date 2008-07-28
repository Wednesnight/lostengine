#ifndef LOST_MODEL_MESH_H
#define LOST_MODEL_MESH_H

#include <vector>
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
      unsigned int vertexCount;
      unsigned int normalCount;

      boost::shared_array<lost::math::Vec3>       vertices;
      boost::shared_array<lost::math::Vec3>       normals;
      std::vector<std::vector<lost::math::Vec3> > faces;

      lost::math::AABB box;

      Mesh(unsigned int inVertexCount, unsigned int inNormalCount)
      {
        setVertexCount(inVertexCount);
        setNormalCount(inNormalCount);
      }
      
      Mesh()
      : vertexCount(0),
        normalCount(0)
      {
      }
      
      ~Mesh() {}

      void clear()
      {
        clearVertices();
        clearNormals();
        clearFaces();
      }

      void clearVertices()
      {
        for (unsigned int idx = 0; idx < vertexCount; ++idx)
        {
          vertices[idx].zero();
        }
      }
      
      void setVertexCount(unsigned int inCount)
      {
        vertexCount = inCount;
        vertices.reset(new lost::math::Vec3[vertexCount]);
        clearVertices();
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
      
      void clearNormals()
      {
        for (unsigned int idx = 0; idx < normalCount; ++idx)
        {
          normals[idx].zero();
        }
      }
      
      void setNormalCount(unsigned int inCount)
      {
        normalCount = inCount;
        normals.reset(new lost::math::Vec3[normalCount]);
        clearNormals();
      }
      
      void setNormal(unsigned int inIndex, lost::math::Vec3& inNormal)
      {
        if (inIndex < normalCount)
        {
          normals[inIndex] = inNormal;
        }
      }
      
      void clearFaces()
      {
        faces.clear();
      }
      
    };
    
    inline std::ostream& operator << (std::ostream& stream, const boost::shared_ptr<Mesh>& m)
    {
      stream << "Mesh:" << std::endl;
      stream << std::endl;
      stream << "  vertexCount: " << m->vertexCount << std::endl;
      for (unsigned int idx = 0; idx < m->vertexCount; ++idx)
        stream << "    vertices[" << idx << "]: " << m->vertices[idx] << std::endl;
      stream << std::endl;
      stream << "  normalCount: " << m->normalCount << std::endl;
      for (unsigned int idx = 0; idx < m->normalCount; ++idx)
        stream << "    normals[" << idx << "]: " << m->normals[idx] << std::endl;
      return stream;
    }
    
  }
}

#endif
