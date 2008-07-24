#ifndef LOST_MODEL_MESH_H
#define LOST_MODEL_MESH_H

#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include "lost/model/Vertex.h"
#include "lost/common/Logger.h"

namespace lost
{
  namespace model
  {
    
    struct Mesh
    {
      unsigned int                vertexCount;
      boost::shared_array<Vertex> vertices;
      
      Mesh(unsigned int inVertexCount)
      : vertexCount(inVertexCount)
      {
        vertices.reset(new Vertex[vertexCount]);
        clear();
      }
      
      ~Mesh() {}

      void clear()
      {
        for (unsigned int idx = 0; idx < vertexCount; ++idx)
        {
          vertices[idx].clear();
        }
      }
      
      void setVertex(unsigned int inIndex, const Vertex& inVertex)
      {
        DOUT("vertices[" << inIndex << "] = " << inVertex);
        if (inIndex < vertexCount)
          vertices[inIndex] = inVertex;
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
