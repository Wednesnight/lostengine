#ifndef LOST_MODEL_RENDERER_H
#define LOST_MODEL_RENDERER_H

#include <vector>
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
        // draw mesh faces as triangles
        glPolygonMode(GL_FRONT, renderModeFront);
        glPolygonMode(GL_BACK, renderModeBack);
        elementBuffer->bind();
        if (material)
        {
          for (MaterialGroups::iterator idx = material->groups.begin(); idx != material->groups.end(); ++idx)
          {
            if ((*idx)->ambient) 
            {
//              DOUT(*((*idx)->ambient));
              glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*)(*idx)->ambient.get());
            }
            if ((*idx)->diffuse)
            {
//              DOUT(*((*idx)->diffuse));
              glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*)(*idx)->diffuse.get());
            }
            if ((*idx)->specular)
            {
//              DOUT(*((*idx)->specular));
              glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*)(*idx)->specular.get());
            }

            elementBuffer->drawRangeElements(GL_TRIANGLES, (*idx)->faceOffset, (*idx)->faceOffset + ((*idx)->faceLength - 1));
          }
        }
        else
        {
          elementBuffer->drawElements(GL_TRIANGLES);
        }
        elementBuffer->unbind();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        vertexBuffer->unbind();
      }
    };

  }
}

#endif
