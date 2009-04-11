#include <boost/shared_ptr.hpp>
#include "lost/gl/State.h"

namespace lost
{
  namespace gl
  {
    
    State::State()
    {
    }
    
    State::State(const ParamPtr& p1)
    {
      param(p1);
    }

    State::State(const ParamPtr& p1, const ParamPtr& p2)
    {
      param(p1);
      param(p2);
    }

    State::State(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3)
    {
      param(p1);
      param(p2);
      param(p3);
    }

    State::State(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4)
    {
      param(p1);
      param(p2);
      param(p3);
      param(p4);
    }

    State::State(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5)
    {
      param(p1);
      param(p2);
      param(p3);
      param(p4);
      param(p5);
    }
    
    State::State(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5,
                 const ParamPtr& p6)
    {
      param(p1);
      param(p2);
      param(p3);
      param(p4);
      param(p5);
      param(p6);
    }
    
    State::State(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5,
                 const ParamPtr& p6, const ParamPtr& p7)
    {
      param(p1);
      param(p2);
      param(p3);
      param(p4);
      param(p5);
      param(p6);
      param(p7);
    }
    
    State::State(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5,
                 const ParamPtr& p6, const ParamPtr& p7, const ParamPtr& p8)
    {
      param(p1);
      param(p2);
      param(p3);
      param(p4);
      param(p5);
      param(p6);
      param(p7);
      param(p8);
    }
    
    State::State(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5,
                 const ParamPtr& p6, const ParamPtr& p7, const ParamPtr& p8, const ParamPtr& p9)
    {
      param(p1);
      param(p2);
      param(p3);
      param(p4);
      param(p5);
      param(p6);
      param(p7);
      param(p8);
      param(p9);
    }
    
    State::State(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5,
                 const ParamPtr& p6, const ParamPtr& p7, const ParamPtr& p8, const ParamPtr& p9, const ParamPtr& p10)
    {
      param(p1);
      param(p2);
      param(p3);
      param(p4);
      param(p5);
      param(p6);
      param(p7);
      param(p8);
      param(p9);
      param(p10);
    }
    
    StatePtr State::create()
    {
      return StatePtr(new State());
    }
    
    StatePtr State::create(const ParamPtr& p1)
    {
      return StatePtr(new State(p1));
    }
    
    StatePtr State::create(const ParamPtr& p1, const ParamPtr& p2)
    {
      return StatePtr(new State(p1, p2));
    }

    StatePtr State::create(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3)
    {
      return StatePtr(new State(p1, p2, p3));
    }

    StatePtr State::create(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4)
    {
      return StatePtr(new State(p1, p2, p3, p4));
    }

    StatePtr State::create(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5)
    {
      return StatePtr(new State(p1, p2, p3, p4, p5));
    }
    
    StatePtr State::create(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5,
                              const ParamPtr& p6)
    {
      return StatePtr(new State(p1, p2, p3, p4, p5, p6));
    }
    
    StatePtr State::create(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5,
                              const ParamPtr& p6, const ParamPtr& p7)
    {
      return StatePtr(new State(p1, p2, p3, p4, p5, p6, p7));
    }
    
    StatePtr State::create(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5,
                              const ParamPtr& p6, const ParamPtr& p7, const ParamPtr& p8)
    {
      return StatePtr(new State(p1, p2, p3, p4, p5, p6, p7, p8));
    }
    
    StatePtr State::create(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5,
                              const ParamPtr& p6, const ParamPtr& p7, const ParamPtr& p8, const ParamPtr& p9)
    {
      return StatePtr(new State(p1, p2, p3, p4, p5, p6, p7, p8, p9));
    }
    
    StatePtr State::create(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5,
                              const ParamPtr& p6, const ParamPtr& p7, const ParamPtr& p8, const ParamPtr& p9, const ParamPtr& p10)
    {
      return StatePtr(new State(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10));
    }
    
    void State::param(const ParamPtr& inParam)
    {
      // we want to overwrite previous values, so don't check for duplicates
      params[inParam->which] = inParam;
    }

  }
}
