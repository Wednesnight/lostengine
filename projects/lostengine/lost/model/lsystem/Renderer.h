#ifndef LOST_MODEL_LSYSTEM_RENDERER_H
#define LOST_MODEL_LSYSTEM_RENDERER_H

#include <boost/shared_ptr.hpp>
#include "lost/model/Mesh.h"
#include "lost/model/Vertex.h"
#include "lost/gl/Canvas.h"
#include "lost/gl/ArrayBuffer.h"

namespace lost
{
  namespace model
  {
    namespace lsystem
    {

      struct Renderer
      {
        boost::shared_ptr<lost::gl::Canvas> canvas;

        boost::shared_ptr<Mesh> mesh;
        boost::shared_ptr<gl::ArrayBuffer<Vertex> > vertexBuffer;

        float size;

        Renderer(const boost::shared_ptr<lost::gl::Canvas>& inCanvas, const boost::shared_ptr<Mesh>& inMesh)
        : canvas(inCanvas),
          mesh(inMesh),
          size(1.0f)
        {
          vertexBuffer.reset(new gl::ArrayBuffer<Vertex>);
          vertexBuffer->bindBufferData(mesh->vertices.get(), mesh->vertexCount);
          vertexBuffer->unbind();
        }
        
        void render()
        {
          static gl::StatePtr newState = gl::State::create(gl::VertexArray::create(true));
          canvas->context->pushState(newState);
          glPushMatrix();GLDEBUG;
          glScalef(size, size, size);
          vertexBuffer->bindVertexPointer();
          vertexBuffer->drawArrays(GL_LINES);
          vertexBuffer->unbind();
          glPopMatrix();GLDEBUG;
          canvas->context->popState();
        }
        
      };
    }
  }
}

#endif
