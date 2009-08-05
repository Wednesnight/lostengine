#ifndef LOST_MESH_RECT_H
#define LOST_MESH_RECT_H

#include "lost/mesh/Mesh.h"
#include "lost/math/Bezier.h"

namespace lost
{
  namespace mesh
  {

    template<typename MESHTYPE, GLenum RectType>
    struct Rectangle : public MESHTYPE
    {
      typedef typename MESHTYPE::VertexType VertexType;
      typedef typename MESHTYPE::IndexType IndexType;

      Rectangle(const math::Rect& rect)
      {
        this->drawMode = RectType;
        boost::uint32_t numVertices = 4;
        boost::uint32_t numIndices = numVertices;
        this->vertices(true);
        this->indices(true);
        this->resetVertices(numVertices);
        this->resetIndices(numIndices);
        this->createVertices(rect);
        this->createIndices();
        this->transferVertices();
        this->transferIndices();
      }

      void createIndices()
      {
        if (RectType == GL_LINE_LOOP)
        {
          this->indexData[0] = 0;
          this->indexData[1] = 1;
          this->indexData[2] = 2;
          this->indexData[3] = 3;
        }
        else if (RectType == GL_TRIANGLE_STRIP)
        {
          this->indexData[0] = 3;
          this->indexData[1] = 0;
          this->indexData[2] = 2;
          this->indexData[3] = 1;
        }
      }

      void createVertices(const math::Rect& rect)
      {
        math::Rect relativeRect(0, 0, rect.width, rect.height);

        this->vertexData[0] = relativeRect.bottomLeft();
        this->vertexData[1] = relativeRect.bottomRight();
        this->vertexData[2] = relativeRect.topRight();
        this->vertexData[3] = relativeRect.topLeft();

        if (rect.x != 0 || rect.y != 0)
        {
          this->transform = math::MatrixTranslation(math::Vec3(rect.x, rect.y, 0));
        }
      }

      static lost::shared_ptr<Rectangle<MESHTYPE, RectType> > create(const math::Rect& rect)
      {
        return lost::shared_ptr<Rectangle<MESHTYPE, RectType> >(new Rectangle<MESHTYPE, RectType>(rect));
      }

    };

    typedef Rectangle<Mesh2D, GL_LINE_LOOP> Rect2D;
    typedef lost::shared_ptr<Rect2D> Rect2DPtr;

    typedef Rectangle<Mesh2D, GL_TRIANGLE_STRIP> FilledRect2D;
    typedef lost::shared_ptr<FilledRect2D> FilledRect2DPtr;



    template<typename MESHTYPE, GLenum RectType>
    struct RoundedRectangle : public MESHTYPE
    {
      typedef typename MESHTYPE::VertexType VertexType;
      typedef typename MESHTYPE::IndexType IndexType;
      
      RoundedRectangle(const math::Rect& rect, const double radius, const unsigned int steps = math::Bezier<VertexType>::defaultSteps())
      {
        this->drawMode = RectType;
        unsigned int offset = (RectType == GL_TRIANGLE_FAN) ? 1 : 0;
        // filled adds center vertex
        boost::uint32_t numVertices = steps*4+offset;
        // filled adds center and closing index
        boost::uint32_t numIndices = numVertices+offset;
        this->vertices(true);
        this->indices(true);
        this->resetVertices(numVertices);
        this->resetIndices(numIndices);
        this->createVertices(rect, radius, steps);
        this->createIndices(steps);
        this->transferVertices();
        this->transferIndices();
      }
      
      void createIndices(const unsigned int steps)
      {
        unsigned int offset = (RectType == GL_TRIANGLE_FAN) ? 1 : 0;
        for (unsigned int idx = 0; idx < steps*4+offset; idx++)
        {
          this->indexData[idx] = idx;
        }
        // add index for closing point
        if (RectType == GL_TRIANGLE_FAN) this->indexData[steps*4+offset] = 1;
      }

      void createVertices(const math::Rect& rect, const double radius, const unsigned int steps)
      {
        math::Rect relativeRect(0, 0, rect.width, rect.height);
        unsigned int offset = (RectType == GL_TRIANGLE_FAN) ? 1 : 0;

        // calculate bezier control points
        float length = sin(45.0)*radius;
        VertexType bl1 = relativeRect.bottomLeft(); VertexType bl0(bl1.x, bl1.y+length); VertexType bl2(bl1.x+length, bl1.y);
        VertexType br1 = relativeRect.bottomRight(); VertexType br0(br1.x-length, br1.y); VertexType br2(br1.x, br1.y+length);
        VertexType tr1 = relativeRect.topRight(); VertexType tr0(tr1.x, tr1.y-length); VertexType tr2(tr1.x-length, tr1.y);
        VertexType tl1 = relativeRect.topLeft(); VertexType tl0(tl1.x+length, tl1.y); VertexType tl2(tl1.x, tl1.y-length);

        // create bezier curves
        math::QuadraticBezier<VertexType>::create(this->vertexData.get(), offset, bl0, bl1, bl2, steps);
        math::QuadraticBezier<VertexType>::create(this->vertexData.get(), steps+offset, br0, br1, br2, steps);
        math::QuadraticBezier<VertexType>::create(this->vertexData.get(), steps*2+offset, tr0, tr1, tr2, steps);
        math::QuadraticBezier<VertexType>::create(this->vertexData.get(), steps*3+offset, tl0, tl1, tl2, steps);

        // add index for center point
        if (RectType == GL_TRIANGLE_FAN) this->vertexData[0] = relativeRect.center();

        if (rect.x != 0 || rect.y != 0)
        {
          this->transform = math::MatrixTranslation(math::Vec3(rect.x, rect.y, 0));
        }
      }
      
      static lost::shared_ptr<RoundedRectangle<MESHTYPE, RectType> > create(const math::Rect& rect, const double radius, 
                                                                            const unsigned int steps = math::Bezier<VertexType>::defaultSteps())
      {
        return lost::shared_ptr<RoundedRectangle<MESHTYPE, RectType> >(new RoundedRectangle<MESHTYPE, RectType>(rect, radius, steps));
      }
      
    };
    
    typedef RoundedRectangle<Mesh2D, GL_LINE_LOOP> RoundedRect2D;
    typedef lost::shared_ptr<RoundedRect2D> RoundedRect2DPtr;
    
    typedef RoundedRectangle<Mesh2D, GL_TRIANGLE_FAN> FilledRoundedRect2D;
    typedef lost::shared_ptr<FilledRoundedRect2D> FilledRoundedRect2DPtr;

  }
}

#endif
