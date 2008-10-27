#include <boost/shared_ptr.hpp>
#include "lost/common/Logger.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/lua/State.h"
#include "lost/guiro/UserInterface.h"

#include "lost/guiro/Screen.h"

namespace lost
{
  namespace guiro
  {

    Screen::Screen(const boost::shared_ptr<lua::State>& inInterpreter, const boost::shared_ptr<lost::common::DisplayAttributes>& inAttributes)
    : interpreter(inInterpreter),
      displayAttributes(inAttributes)
    {
      DOUT("Screen::Screen()");
    }

    Screen::~Screen()
    {
      DOUT("Screen::~Screen()");
    }

    void Screen::validateChild(const boost::shared_ptr<View>& child)
    {
      View::validateChild(child);
      dynamic_cast<UserInterface*>(child.get());
    }

  }
}
