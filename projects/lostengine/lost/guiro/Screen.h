#ifndef LOST_GUIRO_SCREEN_H
#define LOST_GUIRO_SCREEN_H

#include "lost/guiro/View.h"
#include "lost/forward/lost/lua/State.h"
#include "lost/forward/boost/shared_ptr.hpp"

namespace lost
{
  namespace guiro
  {

    struct Screen : public View
    {
      boost::shared_ptr<lua::State>                      interpreter;
      boost::shared_ptr<lost::common::DisplayAttributes> displayAttributes;

      Screen(const boost::shared_ptr<lua::State>& inInterpreter, const boost::shared_ptr<lost::common::DisplayAttributes>& inAttributes);
      virtual ~Screen();

      virtual void validateChild(const boost::shared_ptr<View>& child);

      void reset(const std::string& inFile);
    };

  }
}

#endif
