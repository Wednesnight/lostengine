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

      void State::param(const SharedParam& inParam)
      {
        if (params.find(inParam->which) != params.end()) throw std::runtime_error("duplicate state param");
        params[inParam->which] = inParam;
      }

    }
  }
}
