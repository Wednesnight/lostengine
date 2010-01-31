#ifndef LOST_MESH_CIRCULAR_H
#define LOST_MESH_CIRCULAR_H
/*
#include "lost/mesh/Mesh.h"
#include "lost/math/lmath.h"

namespace lost
{
  namespace mesh
  {

    template<typename MESHTYPE, GLenum CircleType>
    struct Circle : public MESHTYPE
    {
      typedef typename MESHTYPE::VertexType VertexType;
      typedef typename MESHTYPE::IndexType IndexType;

      Circle(const double diameter)
      {
        this->drawMode = CircleType;
        boost::uint32_t numVertices = (CircleType == GL_TRIANGLE_FAN) ? 361 : 360;
        boost::uint32_t numIndices = numVertices;
        this->vertices(true);
        this->indices(true);
        this->resetVertices(numVertices);
        this->resetIndices(numIndices);
        this->createVertices(diameter);
        this->createIndices();
        this->transferVertices();
        this->transferIndices();
      }

      void createIndices()
      {
        unsigned int offset = (CircleType == GL_TRIANGLE_FAN) ? 1 : 0;
        for (unsigned int idx = 0; idx < 360+offset; idx++)
          this->indexData[idx] = idx;
      }

      virtual void createVertices(const double diameter)
      {
        unsigned int offset = (CircleType == GL_TRIANGLE_FAN) ? 1 : 0;
        for (unsigned int idx = offset; idx < 360+offset; idx++)
        {
          float rad = math::deg2rad(idx-offset);
          this->vertexData[idx] = math::Vec2(cos(rad)*diameter, sin(rad)*diameter);
        }
        if (CircleType == GL_TRIANGLE_FAN) this->vertexData[0] = math::Vec2(diameter/2, diameter/2);
      }

      static lost::shared_ptr<Circle<MESHTYPE, CircleType> > create(const double diameter)
      {
        return lost::shared_ptr<Circle<MESHTYPE, CircleType> >(new Circle<MESHTYPE, CircleType>(diameter));
      }

    };

    typedef Circle<Mesh2D, GL_LINE_LOOP> Circle2D;
    typedef lost::shared_ptr<Circle2D> Circle2DPtr;

    typedef Circle<Mesh2D, GL_TRIANGLE_FAN> FilledCircle2D;
    typedef lost::shared_ptr<FilledCircle2D> FilledCircle2DPtr;



    template<typename MESHTYPE, GLenum EllipseType>
    struct Ellipse : public MESHTYPE
    {
      typedef typename MESHTYPE::VertexType VertexType;
      typedef typename MESHTYPE::IndexType IndexType;
      
      Ellipse(const math::Vec2& diameter)
      {
        this->drawMode = EllipseType;
        boost::uint32_t numVertices = (EllipseType == GL_TRIANGLE_FAN) ? 361 : 360;
        boost::uint32_t numIndices = numVertices;
        this->vertices(true);
        this->indices(true);
        this->resetVertices(numVertices);
        this->resetIndices(numIndices);
        this->createVertices(diameter);
        this->createIndices();
        this->transferVertices();
        this->transferIndices();
      }
      
      void createIndices()
      {
        unsigned int offset = (EllipseType == GL_TRIANGLE_FAN) ? 1 : 0;
        for (unsigned int idx = 0; idx < 360+offset; idx++)
          this->indexData[idx] = idx;
      }
      
      virtual void createVertices(const math::Vec2& diameter)
      {
        unsigned int offset = (EllipseType == GL_TRIANGLE_FAN) ? 1 : 0;
        for (unsigned int idx = offset; idx < 360+offset; idx++)
        {
          float rad = math::deg2rad(idx-offset);
          this->vertexData[idx] = math::Vec2(cos(rad)*diameter.x, sin(rad)*diameter.y);
        }
        if (EllipseType == GL_TRIANGLE_FAN) this->vertexData[0] = math::Vec2(diameter.x/2, diameter.y/2);
      }
      
      static lost::shared_ptr<Ellipse<MESHTYPE, EllipseType> > create(const math::Vec2& diameter)
      {
        return lost::shared_ptr<Ellipse<MESHTYPE, EllipseType> >(new Ellipse<MESHTYPE, EllipseType>(diameter));
      }
      
    };
    
    typedef Ellipse<Mesh2D, GL_LINE_LOOP> Ellipse2D;
    typedef lost::shared_ptr<Ellipse2D> Ellipse2DPtr;
    
    typedef Ellipse<Mesh2D, GL_TRIANGLE_FAN> FilledEllipse2D;
    typedef lost::shared_ptr<FilledEllipse2D> FilledEllipse2DPtr;

  }
}
*/
#endif
