#ifndef LOST_MODEL_RENDERER_H
#define LOST_MODEL_RENDERER_H

#include <map>
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

    typedef std::map<boost::shared_ptr<MaterialGroup>, boost::shared_ptr<gl::ElementArrayBuffer<unsigned int> > > ElementBuffers;
    struct RendererOBJ
    {
      boost::shared_ptr<Mesh>        mesh;
      boost::shared_ptr<MaterialOBJ> material;

      boost::shared_ptr<gl::ArrayBuffer<math::Vec3> > vertexBuffer;
      boost::shared_ptr<gl::ArrayBuffer<math::Vec3> > normalBuffer;
      
      // without material
      boost::shared_ptr<gl::ElementArrayBuffer<unsigned int> > elementBuffer;
      // with material
      ElementBuffers elementBuffers;

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

        if (!mesh->normals)
        {
          mesh->normalCount = mesh->vertexCount;
          mesh->normals.reset(new math::Vec3[mesh->normalCount]);
          for (unsigned int vertexIdx = 0; vertexIdx < mesh->vertexCount; ++vertexIdx)
          {
            mesh->normals[vertexIdx].zero();
            for (unsigned int faceIdx = 0; faceIdx < mesh->faceCount; ++faceIdx)
            {
              if (mesh->vertices[vertexIdx] == mesh->vertices[mesh->faces[faceIdx]])
                mesh->normals[vertexIdx] += mesh->vertices[vertexIdx];
            }
            normalise(mesh->normals[vertexIdx]);
          }
        }
        normalBuffer.reset(new gl::ArrayBuffer<math::Vec3>);
        normalBuffer->bindBufferData(mesh->normals.get(), mesh->normalCount);
        
        if (material)
        {
          unsigned int* faces = mesh->faces.get();
          for (MaterialGroups::iterator idx = material->groups.begin(); idx != material->groups.end(); ++idx)
          {
            boost::shared_ptr<gl::ElementArrayBuffer<unsigned int> > buffer(new gl::ElementArrayBuffer<unsigned int>);
            buffer->bindBufferData(&faces[(*idx)->faceOffset], (*idx)->faceLength);
            elementBuffers[*idx] = buffer;
          }
        }
        else
        {
          elementBuffer.reset(new gl::ElementArrayBuffer<unsigned int>);
          elementBuffer->bindBufferData(mesh->faces.get(), mesh->faceCount);
        }
        
        glEnableClientState(GL_VERTEX_ARRAY);GLDEBUG;
        glEnableClientState(GL_NORMAL_ARRAY);GLDEBUG;
      }

      void render()
      {
        glScalef(size, size, size);
        vertexBuffer->bindVertexPointer();
        normalBuffer->bindNormalPointer();
        // draw mesh faces as triangles
        glPolygonMode(GL_FRONT, renderModeFront);
        glPolygonMode(GL_BACK, renderModeBack);
        if (material)
        {
          for (ElementBuffers::iterator idx = elementBuffers.begin(); idx != elementBuffers.end(); ++idx)
          {
            if ((*idx).first->ambient) glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*)(*idx).first->ambient.get());
            if ((*idx).first->diffuse) glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*)(*idx).first->diffuse.get());
            if ((*idx).first->specular) glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*)(*idx).first->specular.get());
//            GLfloat emission[] = {.1f, .1f, .1f, 1.0f};
//            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
            GLfloat shininess[] = {128};
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

            (*idx).second->bind();
            (*idx).second->drawElements(GL_TRIANGLES);
            (*idx).second->unbind();
          }
        }
        else
        {
          elementBuffer->bind();
          elementBuffer->drawElements(GL_TRIANGLES);
          elementBuffer->unbind();
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        normalBuffer->unbind();
        vertexBuffer->unbind();
      }
    };

  }
}

#endif
