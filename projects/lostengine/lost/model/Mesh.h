#ifndef LOST_MODEL_MESH_H
#define LOST_MODEL_MESH_H

#include <vector>
#include "lost/model/Vertex.h"
#include "lost/common/Logger.h"

namespace lost
{
  namespace model
  {
    
    struct Mesh
    {
      unsigned int vertexCount;
      Vertex* vertices;
      
      Mesh(unsigned int inVertexCount)
      : vertexCount(inVertexCount)
      {
        vertices = new Vertex[vertexCount];
        clear();
      }
      
      Mesh(std::vector<Vertex>& inVertices)
      : vertexCount(inVertices.size())
      {
        vertices = new Vertex[vertexCount];
        setVertices(inVertices);
      }
      
      ~Mesh()
      {
        delete[] vertices;
      }
      
      void clear()
      {
        for (unsigned int idx = 0; idx < vertexCount; ++idx)
        {
          vertices[idx].clear();
        }
      }
      
      void setVertices(std::vector<Vertex>& inVertices)
      {
        if (vertexCount == inVertices.size())
        {
          unsigned int vertexIndex = 0;
          for (std::vector<Vertex>::iterator idx = inVertices.begin(); idx != inVertices.end(); ++idx)
          {
            vertices[vertexIndex++] = *idx;
          }
        }
      }
      
      void setVertex(unsigned int inIndex, const Vertex& inVertex)
      {
        DOUT("vertices[" << inIndex << "] = " << inVertex);
        if (inIndex < vertexCount)
          vertices[inIndex] = inVertex;
      }
      
    };
    
    inline std::ostream& operator << (std::ostream& stream, const Mesh& m)
    {
      stream << "vertexCount: " << m.vertexCount << std::endl;
      for (unsigned int idx = 0; idx < m.vertexCount; ++idx)
        stream << "vertices[" << idx << "]: " << m.vertices[idx] << std::endl;
      return stream;
    }
    
  }
}

#endif
