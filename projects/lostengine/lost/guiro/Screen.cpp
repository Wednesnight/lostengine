#include <boost/shared_ptr.hpp>
#include "lost/common/DisplayAttributes.h"

#include "lost/guiro/Screen.h"

namespace lost
{
  namespace guiro
  {
    Screen::Screen(const boost::shared_ptr<lua::State>& inInterpreter, const boost::shared_ptr<lost::common::DisplayAttributes>& inAttributes)
    : interpreter(inInterpreter),
      displayAttributes(inAttributes)
    {
    }

    void Screen::reset(const std::string& inFile)
    {
    }
  }
}
