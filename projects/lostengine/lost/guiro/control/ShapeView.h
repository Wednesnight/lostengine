#ifndef LOST_GUIRO_CONTROL_SHAPEVIEW_H
#define LOST_GUIRO_CONTROL_SHAPEVIEW_H

#include "lost/guiro/control/View.h"
#include <list>
#include "lost/gl/Draw.h"

namespace lost
{
namespace guiro
{
  namespace control
  {
  struct ShapeView : public View
  {
    struct Shape
    {
      lost::common::Color color;
      Shape()
      {
        color = lost::common::whiteColor;
      }
      virtual ~Shape() {}
      virtual void draw(const lost::math::Rect& parentRect, const lost::common::Color& parentColor ) = 0;
    };

    typedef std::list<boost::shared_ptr<Shape> >  ShapeList;
    ShapeList                                     shapes;

    struct Point
    {
      boost::shared_ptr<lost::guiro::common::XDimension> x;
      boost::shared_ptr<lost::guiro::common::YDimension> y;

      Point(boost::shared_ptr<lost::guiro::common::XDimension>  inX,
            boost::shared_ptr<lost::guiro::common::YDimension>	inY)
      : x(inX), y(inY)
      {}

      lost::math::Vec2 operator()(const lost::math::Rect& inParent)
      {
        return lost::math::Vec2((*x)(inParent.width, inParent.height, inParent),
                                (*y)(inParent.width, inParent.height, inParent));
      }
    };


    template<typename XDIM, typename YDIM>
    Point make_point(const XDIM& xdim, const YDIM& ydim)
    {
      return Point(boost::shared_ptr<lost::guiro::common::XDimension>(new XDIM(xdim)),
                   boost::shared_ptr<lost::guiro::common::YDimension>(new YDIM(ydim)));
    }

    struct Line : public Shape
    {
      Point start;
      Point end;
      Line(const Point& inStart, const Point& inEnd)
      : start(inStart), end(inEnd)
      {
      }
      virtual ~Line() {}

      virtual void draw(const lost::math::Rect& parentRect, const lost::common::Color& parentColor )
      {
        lost::common::Color currentColor = parentColor * color;
        lost::math::Vec2 vstart = start(parentRect);
        lost::math::Vec2 vend = end(parentRect);
        lost::gl::setColor(currentColor);
        lost::gl::drawLine(vstart, vend);
      }
    };

    struct FilledRect : public Shape
    {
      common::Area          area; 
      FilledRect(const common::Area& inArea) : area(inArea) {}
      
      virtual ~FilledRect() {}
      
      virtual void draw(const lost::math::Rect& parentRect, const lost::common::Color& parentColor )
      {
        lost::common::Color currentColor = parentColor * color;
        lost::gl::setColor(currentColor);
        lost::math::Rect rect = area(parentRect);
        lost::gl::drawRectFilled(rect);
      }      
    };
    
    static boost::shared_ptr<Line> make_line(const Point& start, const Point& end)
    {
      return boost::shared_ptr<Line>(new Line(start, end));
    }

    ShapeView(const std::string& inName)
    : View(inName)
    {

    }

    template<typename SXDIM, typename SYDIM, typename EXDIM, typename EYDIM>
    void addLine(const SXDIM& startx, const SYDIM& starty, const EXDIM& endx, const EYDIM& endy)
    {
      addShape(make_line(make_point(startx, starty), make_point(endx, endy)));
    }

    void addShape(boost::shared_ptr<Shape> shape)
    {
      shapes.push_back(shape);
    }

    virtual void draw(const lost::math::Rect& parentRect, const lost::common::Color& parentColor )
    {
      lost::common::Color currentColor = parentColor * color;
      lost::math::Rect clippedRect = rect;
      clippedRect.clipTo( parentRect );

      for(ShapeList::iterator i=shapes.begin(); i != shapes.end(); ++i)
      {
        (*i)->draw(clippedRect, currentColor);
      }
    }

  };
  }
  }
}

#endif
