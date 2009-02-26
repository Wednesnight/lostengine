#include "lost/application/gl/State.h"

namespace lost
{
  namespace application
  {
    namespace gl
    {
      
      State::State(const SharedParam& p1)
      {
        param(p1);
      }

      State::State(const SharedParam& p1, const SharedParam& p2)
      {
        param(p1);
        param(p2);
      }

      State::State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3)
      {
        param(p1);
        param(p2);
        param(p3);
      }

      State::State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4)
      {
        param(p1);
        param(p2);
        param(p3);
        param(p4);
      }

      State::State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5)
      {
        param(p1);
        param(p2);
        param(p3);
        param(p4);
        param(p5);
      }
      
      State::State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5, const SharedParam& p6)
      {
        param(p1);
        param(p2);
        param(p3);
        param(p4);
        param(p5);
        param(p6);
      }
      
      State::State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5, const SharedParam& p6, const SharedParam& p7)
      {
        param(p1);
        param(p2);
        param(p3);
        param(p4);
        param(p5);
        param(p6);
        param(p7);
      }
      
      State::State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5, const SharedParam& p6, const SharedParam& p7, const SharedParam& p8)
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
      
      State::State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5, const SharedParam& p6, const SharedParam& p7, const SharedParam& p8, const SharedParam& p9)
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
      
      SharedState State::create(const SharedParam& p1)
      {
        return SharedState(new State(p1));
      }

      SharedState State::create(const SharedParam& p1, const SharedParam& p2)
      {
        return SharedState(new State(p1, p2));
      }

      SharedState State::create(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3)
      {
        return SharedState(new State(p1, p2, p3));
      }

      SharedState State::create(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4)
      {
        return SharedState(new State(p1, p2, p3, p4));
      }

      SharedState State::create(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5)
      {
        return SharedState(new State(p1, p2, p3, p4, p5));
      }
      
      SharedState State::create(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5, const SharedParam& p6)
      {
        return SharedState(new State(p1, p2, p3, p4, p5, p6));
      }
      
      SharedState State::create(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5, const SharedParam& p6, const SharedParam& p7)
      {
        return SharedState(new State(p1, p2, p3, p4, p5, p6, p7));
      }
      
      SharedState State::create(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5, const SharedParam& p6, const SharedParam& p7, const SharedParam& p8)
      {
        return SharedState(new State(p1, p2, p3, p4, p5, p6, p7, p8));
      }
      
      SharedState State::create(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5, const SharedParam& p6, const SharedParam& p7, const SharedParam& p8, const SharedParam& p9)
      {
        return SharedState(new State(p1, p2, p3, p4, p5, p6, p7, p8, p9));
      }
      
      void State::param(const SharedParam& inParam)
      {
        if (params.find(inParam->which) != params.end()) throw std::runtime_error("duplicate state param");
        params[inParam->which] = inParam;
      }

    }
  }
}
