/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/mesh/Sphere.h"
#include "lost/gl/gl.h"
#include "lost/gl/HybridIndexBuffer.h"
#include "lost/gl/HybridVertexBuffer.h"
#include "lost/math/lmath.h"

namespace lost
{
  namespace mesh
  {

    using namespace lost::math;

    Sphere::Sphere(float radius, uint8_t subdivisions)
    {
      this->radius = radius;
      this->subdivisions = subdivisions;

      gl::BufferLayout layout;
      layout.add(gl::ET_vec3_f32, gl::UT_position);
      layout.add(gl::ET_vec3_f32, gl::UT_normal);
      this->resetBuffers(layout, gl::ET_u16);

      indexBuffer->drawMode = GL_TRIANGLES;

      update();
    }

    void Sphere::updateRadius(float radius)
    {
      this->radius = radius;
      update();
    }
    
    void Sphere::updateSubdivisions(uint8_t subdivisions)
    {
      this->subdivisions = subdivisions;
      update();
    }

    void Sphere::update()
    {
      // FIXME: maybe we should make this writeable for spheres with varying rowcount/points per row
      uint8_t pointRows = subdivisions;
      uint8_t pointsPerRow = subdivisions;

      uint16_t numVertices = (pointRows-2)*pointsPerRow + 2;
      uint16_t numIndices = (pointRows-3) * (((pointsPerRow-1) * 6) + 6) + ((pointsPerRow-1) * 3 + 3) * 2;

      this->vertexBuffer->reset(numVertices);
      this->indexBuffer->reset(numIndices);

      double r;
      unsigned int i,j;
      Vec3 v;
      // create vertices, normals
      for (i = 1; i < (pointRows-1); i++)
      {
        v.y = 1.0 - float(i) / float(pointRows-1)*2.0;
        r = sin (acos(v.y));  //radius of the row
        for (j = 0; j < pointsPerRow; j++)
        {
          v.x = r * sin(float(j) / float(pointsPerRow)*M_PI*2.0);
          v.z = r * cos(float(j) / float(pointsPerRow)*M_PI*2.0);

          vertexBuffer->set((i-1)*pointsPerRow+j, gl::UT_normal, v);
          vertexBuffer->set((i-1)*pointsPerRow+j, gl::UT_position, v*radius);
        }
        
      }
      // create highest and lowest vertices
      Vec3 high(0.0f,1.0f,0.0f);
      Vec3 low(0.0f,-1.0f,0.0f);
      vertexBuffer->set((pointRows-2)*pointsPerRow, gl::UT_normal, high);
      vertexBuffer->set((pointRows-2)*pointsPerRow, gl::UT_position, high*radius);
      vertexBuffer->set((pointRows-2)*pointsPerRow+1, gl::UT_normal, low);
      vertexBuffer->set((pointRows-2)*pointsPerRow+1, gl::UT_position, low*radius);
      
      // create indices
      uint16_t index = 0;
      for (i = 1; i < (pointRows-2); i++)
      {
        for (j = 0; j < (pointsPerRow-1); j++)
        {
          indexBuffer->set(index++, gl::UT_index, (uint16_t)((i-1)*pointsPerRow+j));
          indexBuffer->set(index++, gl::UT_index, (uint16_t)((i-1)*pointsPerRow+j+1));
          indexBuffer->set(index++, gl::UT_index, (uint16_t)((i)*pointsPerRow+j));
          
          indexBuffer->set(index++, gl::UT_index, (uint16_t)((i-1)*pointsPerRow+j+1));
          indexBuffer->set(index++, gl::UT_index, (uint16_t)((i)*pointsPerRow+j+1));
          indexBuffer->set(index++, gl::UT_index, (uint16_t)((i)*pointsPerRow+j));
        }
        
        indexBuffer->set(index++, gl::UT_index, (uint16_t)((i-1)*pointsPerRow+pointsPerRow-1));
        indexBuffer->set(index++, gl::UT_index, (uint16_t)((i-1)*pointsPerRow));
        indexBuffer->set(index++, gl::UT_index, (uint16_t)((i)*pointsPerRow+j));
        
        indexBuffer->set(index++, gl::UT_index, (uint16_t)((i)*pointsPerRow));
        indexBuffer->set(index++, gl::UT_index, (uint16_t)((i-1)*pointsPerRow));
        indexBuffer->set(index++, gl::UT_index, (uint16_t)((i)*pointsPerRow+j));
      }		
      
      // triangles to highest and deepest vertices
      for (j = 0; j< (pointsPerRow-1); j++)
      {
        indexBuffer->set(index++, gl::UT_index, (uint16_t)j);
        indexBuffer->set(index++, gl::UT_index, (uint16_t)(j+1));
        indexBuffer->set(index++, gl::UT_index, (uint16_t)((pointRows-2)*pointsPerRow));
      }
      indexBuffer->set(index++, gl::UT_index, (uint16_t)j);
      indexBuffer->set(index++, gl::UT_index, (uint16_t)0);
      indexBuffer->set(index++, gl::UT_index, (uint16_t)((pointRows-2)*pointsPerRow));
      
      for (j = 0; j< (pointsPerRow-1); j++)
      {
        indexBuffer->set(index++, gl::UT_index, (uint16_t)((pointRows-3)*pointsPerRow+j));
        indexBuffer->set(index++, gl::UT_index, (uint16_t)((pointRows-3)*pointsPerRow+j+1));
        indexBuffer->set(index++, gl::UT_index, (uint16_t)((pointRows-2)*pointsPerRow+1));
      }
      indexBuffer->set(index++, gl::UT_index, (uint16_t)((pointRows-3)*pointsPerRow+j));
      indexBuffer->set(index++, gl::UT_index, (uint16_t)((pointRows-3)*pointsPerRow));
      indexBuffer->set(index++, gl::UT_index, (uint16_t)((pointRows-2)*pointsPerRow+1));
    }
    
  }
}
