#ifndef GL_STATE_H
#define GL_STATE_H

#include <map>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include "lost/gl/StateParam.h"

namespace lost
{
  namespace gl
  {
    
    struct State;
    typedef boost::shared_ptr<State> SharedState;

    struct State
    {
    private:
      State(const SharedParam& p1);
      State(const SharedParam& p1, const SharedParam& p2);
      State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3);
      State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4);
      State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5);
      State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5, const SharedParam& p6);
      State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5, const SharedParam& p6, const SharedParam& p7);
      State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5, const SharedParam& p6, const SharedParam& p7, const SharedParam& p8);
      State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5, const SharedParam& p6, const SharedParam& p7, const SharedParam& p8, const SharedParam& p9);
    public:
      std::map<GLenum, SharedParam> params;
      
      static SharedState create(const SharedParam& p1);
      static SharedState create(const SharedParam& p1, const SharedParam& p2);
      static SharedState create(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3);
      static SharedState create(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4);
      static SharedState create(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5);
      static SharedState create(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5, const SharedParam& p6);
      static SharedState create(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5, const SharedParam& p6, const SharedParam& p7);
      static SharedState create(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5, const SharedParam& p6, const SharedParam& p7, const SharedParam& p8);
      static SharedState create(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4, const SharedParam& p5, const SharedParam& p6, const SharedParam& p7, const SharedParam& p8, const SharedParam& p9);
      
      void param(const SharedParam& inParam);
    };

  }
}

#endif
