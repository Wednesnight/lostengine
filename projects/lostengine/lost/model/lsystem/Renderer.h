#ifndef LOST_MODEL_LSYSTEM_RENDERER_H
#define LOST_MODEL_LSYSTEM_RENDERER_H

#include <boost/shared_ptr.hpp>
#include "lost/model/Mesh.h"
#include "lost/model/Vertex.h"
#include "lost/gl/Context.h"

namespace lost
{
  namespace model
  {
    namespace lsystem
    {

      struct Renderer
      {
        boost::shared_ptr<lost::gl::Context> context;

        boost::shared_ptr<Mesh> mesh;
        boost::shared_ptr<gl::ArrayBuffer<Vertex> > vertexBuffer;

        Renderer(const boost::shared_ptr<Mesh>& inMesh)
        : context(lost::gl::Context::instance()),
          mesh(inMesh)
        {
          vertexBuffer.reset(new gl::ArrayBuffer<Vertex>);
          vertexBuffer->bindBufferData(mesh->vertices.get(), mesh->vertexCount);
          vertexBuffer->unbind();
        }
        
        void render()
        {
          boost::shared_ptr<lost::gl::State> newState = context->copyState();
          newState->vertexArray = true;
          context->pushState(newState);
          glPushMatrix();GLDEBUG;
          vertexBuffer->bindVertexPointer();
          vertexBuffer->drawArrays(GL_LINES);
          vertexBuffer->unbind();
          glPopMatrix();GLDEBUG;
          context->popState();
        }
        
      };
    }
  }
}

#endif
