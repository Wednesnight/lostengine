#ifndef LOST_GUIRO_COMMON_AREA_H
#define LOST_GUIRO_COMMON_AREA_H

#include "lost/guiro/common/Dimension.h"
#include "lost/math/Rect.h"
#include <boost/shared_ptr.hpp>

namespace lost
{
  namespace guiro
  {
    namespace common
    {

    struct Area
    {

      Area()
      {
        x = boost::shared_ptr<lost::guiro::common::XDimension>(new lost::guiro::common::xabs(0));
        y = boost::shared_ptr<lost::guiro::common::YDimension>(new lost::guiro::common::yabs(0));
        width = boost::shared_ptr<lost::guiro::common::WDimension>(new lost::guiro::common::wrel(1));
        height = boost::shared_ptr<lost::guiro::common::HDimension>(new lost::guiro::common::hrel(1));
      };

      Area(boost::shared_ptr<lost::guiro::common::XDimension>  inx,
           boost::shared_ptr<lost::guiro::common::YDimension>	iny,
           boost::shared_ptr<lost::guiro::common::WDimension>	inw,
           boost::shared_ptr<lost::guiro::common::HDimension>	inh)
      : x(inx), y(iny), width(inw), height(inh) {}

      lost::math::Rect operator() (const lost::math::Rect& parent) const
      {
        float curwidth = (*width)(parent.width);
        float curheight = (*height)(parent.height);

        return lost::math::Rect((*x)(curwidth, curheight, parent),
                           (*y)(curwidth, curheight, parent),
                           curwidth,
                           curheight);
      }

      lost::math::Rect operator() () const
      {
        return lost::math::Rect((*x)(),
                           (*y)(),
                           (*width)(),
                           (*height)());
      }


      boost::shared_ptr<lost::guiro::common::XDimension>	x;
      boost::shared_ptr<lost::guiro::common::YDimension>	y;
      boost::shared_ptr<lost::guiro::common::WDimension>	width;
      boost::shared_ptr<lost::guiro::common::HDimension>	height;
    };

    template<typename XDIM, typename YDIM, typename WDIM, typename HDIM>
    lost::guiro::common::Area make_area(const XDIM& xdim, const YDIM& ydim, const WDIM& wdim, const HDIM& hdim)
    {
      return guiro::common::Area(boost::shared_ptr<lost::guiro::common::XDimension>(new XDIM(xdim)),
                         boost::shared_ptr<lost::guiro::common::YDimension>(new YDIM(ydim)),
                         boost::shared_ptr<lost::guiro::common::WDimension>(new WDIM(wdim)),
                         boost::shared_ptr<lost::guiro::common::HDimension>(new HDIM(hdim))
                         );
    }

    template<typename XDIM, typename YDIM>
    lost::guiro::common::Area make_area(const XDIM& xdim, const YDIM& ydim)
    {
      return guiro::common::Area(boost::shared_ptr<lost::guiro::common::XDimension>(new XDIM(xdim)),
                         boost::shared_ptr<lost::guiro::common::YDimension>(new YDIM(ydim)),
                         boost::shared_ptr<lost::guiro::common::WDimension>(new lost::guiro::common::wabs(0)),
                         boost::shared_ptr<lost::guiro::common::HDimension>(new lost::guiro::common::habs(0))
                         );
    }
  }
  }
}

#endif
