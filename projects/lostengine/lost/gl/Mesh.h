#ifndef LOST_GL_MESH_H
#define LOST_GL_MESH_H

#include <boost/cstdint.hpp>
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
      Mesh(boost::uint32_t inVertexCount, boost::uint32_t inFaceCount)
      : vertexCount(inVertexCount),
        faceCount(inFaceCount)
      {
        setVertexCount(vertexCount);
        setFaceCount(faceCount);
      }

      Mesh(boost::uint32_t inVertexCount, boost::uint32_t inTexcoordCount, boost::uint32_t inFaceCount, lost::shared_ptr<Texture> inTexture)
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

      void setVertexCount(boost::uint32_t inVertexCount)
      {
        vertices.reset(new VertexType[inVertexCount]);
      }

      void setVertex(boost::uint32_t index, const VertexType& value)
      {
        if (index < vertexCount) vertices[index] = value;
      }

      void setTexcoordCount(boost::uint32_t inTexcoordCount)
      {
        texcoords.reset(new VertexType[inTexcoordCount]);
      }

      void setTexcoord(boost::uint32_t index, const VertexType& value)
      {
        if (index < texcoordCount) texcoords[index] = value;
      }

      void setFaceCount(boost::uint32_t inFaceCount)
      {
        faces.reset(new boost::uint8_t[inFaceCount]);
      }

      void setFace(boost::uint32_t index, const boost::uint8_t value)
      {
        if (index < faceCount) faces[index] = value;
      }
      
      boost::uint32_t vertexCount;
      boost::uint32_t texcoordCount;
      boost::uint32_t faceCount;

      boost::shared_array<VertexType>     vertices;
      boost::shared_array<VertexType>     texcoords;
      boost::shared_array<boost::uint8_t> faces;
      lost::shared_ptr<gl::Texture>      texture;
    };

    typedef Mesh<math::Vec2> Mesh2D;
    typedef Mesh<math::Vec3> Mesh3D;
  }
}

#endif
