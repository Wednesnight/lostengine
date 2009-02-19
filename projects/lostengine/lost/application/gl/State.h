#ifndef GL_STATE_H
#define GL_STATE_H

#include <map>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include "lost/application/gl/StateParam.h"

namespace lost
{
  namespace application
  {
    namespace gl
    {
      
      struct State;
      typedef boost::shared_ptr<State> SharedState;

      struct State
      {
      private:
        std::map<GLenum, SharedParam> params;

        State(const SharedParam& p1);
        State(const SharedParam& p1, const SharedParam& p2);
        State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3);
        State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4);
      public:
        static SharedState create(const SharedParam& p1);
        static SharedState create(const SharedParam& p1, const SharedParam& p2);
        static SharedState create(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3);
        static SharedState create(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4);
        
        void param(const SharedParam& inParam);
      };

    }
  }
}

#endif
