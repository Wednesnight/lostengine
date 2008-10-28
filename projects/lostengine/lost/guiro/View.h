#ifndef LOST_GUIRO_VIEW_H
#define LOST_GUIRO_VIEW_H

#include <map>
#include <boost/enable_shared_from_this.hpp>
#include "lost/event/EventDispatcher.h"
#include "lost/forward/boost/shared_ptr.hpp"

namespace lost
{
  namespace guiro
  {

    struct View : public lost::event::EventDispatcher, boost::enable_shared_from_this<View>
    {
      typedef std::map<std::string, boost::shared_ptr<View> > ChildHierarchy;

      View();
      virtual ~View();

      virtual void dispatchEvent(lost::event::EventPtr event);

      virtual void appendChild(const boost::shared_ptr<View>& child);
      virtual void removeChild(const boost::shared_ptr<View>& child);
      virtual void validateChild(const boost::shared_ptr<View>& child);

      std::string             id;
      ChildHierarchy          children;
      boost::shared_ptr<View> parentView;
    };

  }
}

#endif
