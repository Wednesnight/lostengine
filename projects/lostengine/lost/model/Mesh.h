#ifndef LOST_MODEL_MESH_H
#define LOST_MODEL_MESH_H

#include <vector>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include "lost/common/Logger.h"
#include "lost/math/AABB.h"
#include "lost/model/Vertex.h"
#include "lost/math/Vec3.h"

namespace lost
{
  namespace model
  {

    struct Mesh
    {
      unsigned int vertexCount;
      unsigned int faceCount;
      unsigned int normalCount;

      boost::shared_array<Vertex>       vertices;
      boost::shared_array<unsigned int> faces;
      boost::shared_array<math::Vec3>   normals;

      lost::math::AABB box;

      Mesh(unsigned int inVertexCount, unsigned int inFaceCount)
      {
        setVertexCount(inVertexCount);
        setFaceCount(inFaceCount);
      }
      
      Mesh()
      : vertexCount(0),
        faceCount(0)
      {
      }
      
      ~Mesh() {}

      void clear()
      {
        clearVertices();
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
        vertices.reset(new Vertex[vertexCount]);
        clearVertices();
      }
      
      void setVertex(unsigned int inIndex, Vertex& inVertex)
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
      
      void clearFaces()
      {
        for (unsigned int idx = 0; idx < faceCount; ++idx)
        {
          faces[idx] = 0;
        }
      }
      
      void setFaceCount(unsigned int inCount)
      {
        faceCount = inCount;
        faces.reset(new unsigned int[faceCount]);
        clearFaces();
      }
      
      void setFacePoint(unsigned int inIndex, unsigned int& inFacePoint)
      {
        if (inIndex < faceCount)
        {
          faces[inIndex] = inFacePoint;
        }
      }
      
    };
    
    inline std::ostream& operator << (std::ostream& stream, const boost::shared_ptr<Mesh>& m)
    {
      stream << "Mesh:" << std::endl;
      stream << std::endl;
      stream << "  vertexCount: " << m->vertexCount << std::endl;
//      for (unsigned int idx = 0; idx < m->vertexCount; ++idx)
//        stream << "    vertices[" << idx << "]: " << m->vertices[idx] << std::endl;
      stream << std::endl;
      stream << "  faceCount: " << m->faceCount << std::endl;
//      for (unsigned int idx = 0; idx < m->faceCount; ++idx)
//        stream << "    faces[" << idx << "]: " << m->faces[idx] << std::endl;
      return stream;
    }
    
  }
}

#endif
