#ifndef LOST_GL_MESH_H
#define LOST_GL_MESH_H

#include <stdint.h>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include "lost/gl/Texture.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"

namespace lost
{
  namespace gl
  {

    template <typename VertexType>
    struct Mesh
    {
      Mesh(uint32_t inVertexCount, uint32_t inTexcoordCount, uint32_t inFaceCount, boost::shared_ptr<Texture> inTexture)
      : vertexCount(inVertexCount),
        texcoordCount(inTexcoordCount),
        faceCount(inFaceCount),
        texture(inTexture)
      {
        setVertexCount(vertexCount);
        setTexcoordCount(texcoordCount);
        setFaceCount(faceCount);
      }

      virtual ~Mesh()
      {
      }

      void setVertexCount(uint32_t inVertexCount)
      {
        vertices.reset(new VertexType[inVertexCount]);
      }

      void setVertex(uint32_t index, const VertexType& value)
      {
        if (index < vertexCount) vertices[index] = value;
      }

      void setTexcoordCount(uint32_t inTexcoordCount)
      {
        texcoords.reset(new VertexType[inTexcoordCount]);
      }

      void setTexcoord(uint32_t index, const VertexType& value)
      {
        if (index < texcoordCount) texcoords[index] = value;
      }

      void setFaceCount(uint32_t inFaceCount)
      {
        faces.reset(new uint8_t[inFaceCount]);
      }

      void setFace(uint32_t index, const uint8_t value)
      {
        if (index < faceCount) faces[index] = value;
      }
      
      uint32_t vertexCount;
      uint32_t texcoordCount;
      uint32_t faceCount;

      boost::shared_array<VertexType> vertices;
      boost::shared_array<VertexType> texcoords;
      boost::shared_array<uint8_t>    faces;
      boost::shared_ptr<gl::Texture>  texture;
    };

    typedef Mesh<math::Vec2> Mesh2D;
    typedef Mesh<math::Vec3> Mesh3D;
  }
}

#endif
