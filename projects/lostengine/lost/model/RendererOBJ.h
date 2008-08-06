#ifndef LOST_MODEL_RENDERER_H
#define LOST_MODEL_RENDERER_H

#include <boost/shared_ptr.hpp>

#include "lost/model/Mesh.h"
#include "lost/model/MaterialOBJ.h"

#include "lost/gl/gl.h"
#include "lost/gl/ArrayBuffer.h"
#include "lost/gl/ElementArrayBuffer.h"
#include "lost/math/Vec3.h"

namespace lost
{
  namespace model
  {

    struct RendererOBJ
    {
      boost::shared_ptr<Mesh>        mesh;
      boost::shared_ptr<MaterialOBJ> material;

      boost::shared_ptr<gl::ArrayBuffer<math::Vec3> >          vertexBuffer;
      boost::shared_ptr<gl::ElementArrayBuffer<unsigned int> > elementBuffer;

      float  size;
      GLenum renderModeFront;
      GLenum renderModeBack;
      
      RendererOBJ(boost::shared_ptr<Mesh> inMesh, boost::shared_ptr<MaterialOBJ> inMaterial)
      : mesh(inMesh),
        material(inMaterial),
        size(1.0f),
        renderModeFront(GL_FILL),
        renderModeBack(GL_FILL)
      {
        vertexBuffer.reset(new gl::ArrayBuffer<math::Vec3>);
        vertexBuffer->bindBufferData(mesh->vertices.get(), mesh->vertexCount);
        
        elementBuffer.reset(new gl::ElementArrayBuffer<unsigned int>);
        elementBuffer->bindBufferData(mesh->faces.get(), mesh->faceCount);
        
        glEnableClientState(GL_VERTEX_ARRAY);GLDEBUG;
      }

      void render()
      {
        glScalef(size, size, size);
        vertexBuffer->bindVertexPointer();
        elementBuffer->bind();
        // draw mesh faces as triangles
        glPolygonMode(GL_FRONT, renderModeFront);
        glPolygonMode(GL_BACK, renderModeBack);
        elementBuffer->drawElements(GL_TRIANGLES);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        elementBuffer->unbind();
        vertexBuffer->unbind();
      }
    };

  }
}

#endif
