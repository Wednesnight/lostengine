#ifndef LOST_MODEL_LSYSTEM_RENDERER_H
#define LOST_MODEL_LSYSTEM_RENDERER_H

#include <boost/shared_ptr.hpp>
#include "lost/model/Mesh.h"
#include "lost/model/Vertex.h"

namespace lost
{
  namespace model
  {
    namespace lsystem
    {

      struct Renderer
      {
        boost::shared_ptr<Mesh> mesh;
        boost::shared_ptr<gl::ArrayBuffer<Vertex> > vertexBuffer;

        Renderer(const boost::shared_ptr<Mesh>& inMesh)
        : mesh(inMesh)
        {
          vertexBuffer.reset(new gl::ArrayBuffer<Vertex>);
          vertexBuffer->bindBufferData(mesh->vertices.get(), mesh->vertexCount / 2);
          glEnableClientState(GL_VERTEX_ARRAY);GLDEBUG;
        }
        
        void render()
        {
          glPushMatrix();GLDEBUG;
          vertexBuffer->bindVertexPointer();
          vertexBuffer->drawArrays(GL_LINES);
          vertexBuffer->unbind();
          glPopMatrix();GLDEBUG;
        }
        
      };
    }
  }
}

#endif
