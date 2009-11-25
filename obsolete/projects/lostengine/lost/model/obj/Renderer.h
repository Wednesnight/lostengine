#ifndef LOST_MODEL_OBJ_RENDERER_H
#define LOST_MODEL_OBJ_RENDERER_H

#include <map>

#include "lost/model/Mesh.h"
#include "lost/model/obj/Material.h"

#include "lost/gl/gl.h"
#include "lost/gl/ArrayBuffer.h"
#include "lost/gl/Canvas.h"
#include "lost/gl/Context.h"
#include "lost/gl/ElementArrayBuffer.h"
#include "lost/math/Vec3.h"

namespace lost
{
  namespace model
  {
    namespace obj
    {

      typedef std::map<lost::shared_ptr<MaterialGroup>, lost::shared_ptr<gl::ElementArrayBuffer<unsigned short> > > ElementBuffers;
      struct Renderer
      {
        lost::shared_ptr<lost::gl::Canvas> canvas;

        lost::shared_ptr<Mesh>     mesh;
        lost::shared_ptr<Material> material;

        lost::shared_ptr<gl::ArrayBuffer<math::Vec3> > vertexBuffer;
        lost::shared_ptr<gl::ArrayBuffer<math::Vec3> > normalBuffer;
        
        // without material
        lost::shared_ptr<gl::ElementArrayBuffer<unsigned short> > elementBuffer;
        // with material
        ElementBuffers elementBuffers;

        float  size;
#if TARGET_OPENGL
        GLenum renderModeFront;
        GLenum renderModeBack;
#endif

        Renderer(const lost::shared_ptr<lost::gl::Canvas>& inCanvas, lost::shared_ptr<Mesh> inMesh, lost::shared_ptr<Material> inMaterial)
        : canvas(inCanvas),
          mesh(inMesh),
          material(inMaterial),
          size(1.0f)
#if TARGET_OPENGL
          ,renderModeFront(GL_FILL)
          ,renderModeBack(GL_FILL)
#endif
        {
          DOUT("constructor start");
          DOUT("bind vertex buffer data start");
          vertexBuffer.reset(new gl::ArrayBuffer<math::Vec3>);
          vertexBuffer->bindBufferData(mesh->vertices.get(), mesh->vertexCount);
          vertexBuffer->unbind();
          DOUT("bind vertex buffer data end");

          if (!mesh->normals)
          {
            DOUT("calculate normals start");
            DOUT("vertices: " << mesh->vertexCount << " faces: " << mesh->faceCount << " num checks: " << mesh->vertexCount + mesh->faceCount);
            mesh->normalCount = mesh->vertexCount;
            mesh->normals.reset(new math::Vec3[mesh->normalCount]);
            for (unsigned int faceIdx = 0; faceIdx < mesh->faceCount; ++faceIdx)
            {
              mesh->normals[mesh->faces[faceIdx]] += mesh->vertices[mesh->faces[faceIdx]];
            }
            for (unsigned int normalIdx = 0; normalIdx < mesh->normalCount; ++normalIdx)
            {
              normalise(mesh->normals[normalIdx]);
            }
            DOUT("calculate normals end");
          }
          DOUT("bind normal data start");
          normalBuffer.reset(new gl::ArrayBuffer<math::Vec3>);
          normalBuffer->bindBufferData(mesh->normals.get(), mesh->normalCount);
          normalBuffer->unbind();
          DOUT("bind normal data end");
          
          if (material && material->groups.size() > 0)
          {
            DOUT("create element buffers start");
            unsigned short* faces = mesh->faces.get();
            for (MaterialGroups::iterator idx = material->groups.begin(); idx != material->groups.end(); ++idx)
            {
              lost::shared_ptr<gl::ElementArrayBuffer<unsigned short> > buffer(new gl::ElementArrayBuffer<unsigned short>);
              buffer->bindBufferData(&faces[(*idx)->faceOffset], (*idx)->faceLength);
              elementBuffers[*idx] = buffer;
              buffer->unbind();
            }
            DOUT("create element buffers end");
          }
          else
          {
            DOUT("create single element buffer start");
            elementBuffer.reset(new gl::ElementArrayBuffer<unsigned short>);
            elementBuffer->bindBufferData(mesh->faces.get(), mesh->faceCount);
            elementBuffer->unbind();
            DOUT("create single element buffer end");          
          }
          DOUT("constructor end");
        }

        void render()
        {
          static gl::StatePtr newState = gl::State::create(gl::NormalArray::create(true), gl::VertexArray::create(true));
          canvas->context->pushState(newState);
          glPushMatrix();
          glScalef(size, size, size);
          vertexBuffer->bindVertexPointer();
          normalBuffer->bindNormalPointer();
          // draw mesh faces as triangles
#if TARGET_OPENGL
          glPolygonMode(GL_FRONT, renderModeFront);
          glPolygonMode(GL_BACK, renderModeBack);
#endif
          if (material && material->groups.size() > 0)
          {
            for (ElementBuffers::iterator idx = elementBuffers.begin(); idx != elementBuffers.end(); ++idx)
            {
              if ((*idx).first->ambient) glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*)(*idx).first->ambient.get());
              if ((*idx).first->diffuse) glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*)(*idx).first->diffuse.get());
              if ((*idx).first->specular) glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*)(*idx).first->specular.get());
//              GLfloat emission[] = {.1f, .1f, .1f, 1.0f};
//              glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
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
#if TARGET_OPENGL
          glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
          normalBuffer->unbind();
          vertexBuffer->unbind();
          glPopMatrix();
          canvas->context->popState();
        }
        
        void renderNormals()
        {
          if (mesh->normals)
          {
            glPushMatrix();
            glScalef(size, size, size);
            canvas->setColor(common::redColor);
            for (unsigned int idx = 0; idx < mesh->normalCount; ++idx)
            {
              math::Vec3 offset(mesh->vertices[mesh->faces[idx*3]]);
              math::Vec3 normal(offset + mesh->normals[idx] * 0.1f);
              canvas->drawLine(offset, normal);
            }
            glPopMatrix();
          }
        }
        
        void renderAABB()
        {
          glPushMatrix();
          glScalef(size, size, size);
          canvas->setColor(common::whiteColor);
          canvas->drawAABB(mesh->box);
          glPopMatrix();
        }
      };

    }
  }
}

#endif