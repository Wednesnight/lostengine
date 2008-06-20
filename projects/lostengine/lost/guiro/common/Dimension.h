#ifndef LOST_GUIRO_COMMON_DIMENSION_H
#define LOST_GUIRO_COMMON_DIMENSION_H

#include "lost/math/Rect.h"

namespace lost
{
  namespace guiro
  {
    namespace common
    {

    /** base class for all dimensions.
     * Specialized dimensions are used to build a parametrizable description of one of the four attributes
     * of a rectangular area (left edge x position, bottom edge y position, width, height).
     * All dimensions share two attributes: a relative and an absolute value. How these values are interpreted
     * is always up to the specialized subclass.
     * An example for a simple x dimension would be: rel = 0.5, abs = 10 which could be interpreted as
     * "my x position is half the width of the parent (0.5 = 50%) plus 10 absolute pixels"
     * Some dimensions might require additional parameters to calculate their value.
     */
    struct Dimension
    {
      Dimension() : relative(0.0f), absolute(0.0f) {}
      Dimension(float r, float a) : relative(r), absolute(a) {}
      virtual ~Dimension() {}
      float operator()() const { return absolute; };
      float operator()(float parent) const { return relative*parent+absolute; };
      float relative;
      float absolute;
    };

    //////////////////////////////////////////////////////
    // X
    //////////////////////////////////////////////////////

    /** base class for all dimensions describing the left edge of an area
     */
    struct XDimension : public Dimension
    {
      XDimension() {}
      XDimension(float r, float a) : Dimension(r,a) {}
      virtual ~XDimension() {}
      float operator()() const { return this->Dimension::operator()(); };
      virtual float operator()(float curwidth, float curheight, const lost::math::Rect& parent) const = 0;
    };

    /** most flexible and general definition for the left edge of an area
     */
    struct leftedge : public XDimension
    {
      leftedge() {}
      leftedge(float r, float a) : XDimension(r, a) {}
      virtual ~leftedge() {}
      /** calculates the current left edge
       * @param curwidth current width of the object  (NOT parent width)
       * @param curheight current height of the object (NOT parent height)
       * @param parent current height of the object
       */
      float operator()(float curwidth, float curheight, const lost::math::Rect& parent) const
      {
        return parent.x + relative*parent.width+absolute;
      };
    };

    /** centers the x coordinate inside the parents width
     * use the relative amount to position the coordinate within the parents width
     * .5 = 50% = center
     * .25 = left quarter
     * etc.
     * if you want to space out 3 objects that are each a third of the parents width, you'd use
     * the following 3 x coords for the objects:
     * xcenter(0) xcenter(.3) xcenter(.6)
     *
     */
    struct xcenter : public XDimension
    {
      xcenter(float r= .5f, float a = 0.0f) : XDimension(r, a) {}
      float operator()(float curwidth, float curheight, const lost::math::Rect& parent) const
      {
        return parent.x + (parent.width-curwidth)*relative + absolute;
      };
    };

    /** creates an x coord that is equal to the parents left edge
     */
    // FIXME: how can we use absolute/relative values here sensibly?
    struct xalignleft : public XDimension
    {
      xalignleft(float r = 1.0f, float a = 0.0f) : XDimension(r, a) {}
      float operator()(float curwidth, float curheight, const lost::math::Rect& parent) const
      {
        return parent.x;
      };
    };

    /** creates an xcoord so that the current objects rects right edge is aligned
     *  with the parents right edge.
     *  For this to work, the current width of the object has to be calculated first.
     */
    struct xalignright : public XDimension
    {
      xalignright(float a= 0.0f) : XDimension(0.0f, a) {}
      float operator()(float curwidth, float curheight, const lost::math::Rect& parent) const
      {
        return ((parent.x+parent.width)-curwidth);//+absolute;
      };
    };

    struct xabs : public leftedge
    {
      xabs() {}
      xabs(float x) : leftedge(0.0f, x) {}
    };

    struct xrel : public leftedge
    {
      xrel() {}
      xrel(float x) : leftedge(x, 0.0f) {}
    };

    //////////////////////////////////////////////////////
    // Y
    //////////////////////////////////////////////////////

    struct YDimension : public Dimension
    {
      YDimension() {}
      YDimension(float r, float a) : Dimension(r,a) {}
      virtual ~YDimension() {}
      float operator()() const { return this->Dimension::operator()(); };
      virtual float operator()(float curwidth, float curheight, const lost::math::Rect& parent) const = 0;
    };

    struct bottomedge : public YDimension
    {
      bottomedge() {}
      bottomedge(float r, float a) : YDimension(r, a) {}
      virtual ~bottomedge() {}
      float operator()(float curwidth, float curheight, const lost::math::Rect& parent) const
      {
        return parent.y + relative*parent.height+absolute;
      };
    };

    struct ycenter : public YDimension
    {
      ycenter(float r=.5f, float a=0.0f) : YDimension(r, a) {}
      float operator()(float curwidth, float curheight, const lost::math::Rect& parent) const
      {
        return parent.y + (parent.height+-curheight)*relative + absolute;
      };
    };

    struct yalignbottom : public YDimension
    {
      yalignbottom() {}
      float operator()(float curwidth, float curheight, const lost::math::Rect& parent) const
      {
        return parent.y;
      };
    };

    struct yaligntop : public YDimension
    {
      yaligntop(float absval=0.0f) : YDimension(0.0f, absval) {}
      float operator()(float curwidth, float curheight, const lost::math::Rect& parent) const
      {
        return ((parent.y+parent.height)-curheight)+absolute;
      };
    };

    struct yabs : public bottomedge
    {
      yabs() {}
      yabs(float y) : bottomedge(0.0f, y) {}
    };

    struct yrel : public bottomedge
    {
      yrel() {}
      yrel(float y) : bottomedge(y, 0.0f) {}
    };

    //////////////////////////////////////////////////////
    // W
    //////////////////////////////////////////////////////

    struct WDimension : public Dimension
    {
      WDimension() {}
      WDimension(float r, float a) : Dimension(r,a) {}
      virtual ~WDimension() {}
    };

    struct width : public WDimension
    {
      width() {}
      width(float r, float a) : WDimension(r, a) {}
      virtual ~width() {}
    };

    struct wabs : public width
    {
      wabs() {}
      wabs(float w) : width(0.0f, w) {}
    };

    struct wrel : public width
    {
      wrel() {}
      wrel(float r, float a = 0.0f) : width(r, a) {}
    };

    //////////////////////////////////////////////////////
    // H
    //////////////////////////////////////////////////////

    struct HDimension : public Dimension
    {
      HDimension() {}
      virtual ~HDimension() {}
      HDimension(float r, float a) : Dimension(r,a) {}
    };

    struct height : public HDimension
    {
      height() {}
      height(float r, float a) : HDimension(r, a) {}
      virtual ~height() {}
    };

    struct habs : public height
    {
      habs() {}
      habs(float h) : height(0.0f, h) {}
    };

    struct hrel : public height
    {
      hrel() {}
      hrel(float r) : height(r, 0.0f) {}
    };
    }
  }
}

#endif
