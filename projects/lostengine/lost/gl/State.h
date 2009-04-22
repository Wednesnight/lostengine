#ifndef GL_STATE_H
#define GL_STATE_H

#include <map>
#include <stdexcept>
#include "lost/gl/StateParam.h"

namespace lost
{
  namespace gl
  {
    
    struct State;
    typedef lost::shared_ptr<State> StatePtr;

    struct State
    {
    private:
      State();
      State(const ParamPtr& p1);
      State(const ParamPtr& p1, const ParamPtr& p2);
      State(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3);
      State(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4);
      State(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5);
      State(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5, const ParamPtr& p6);
      State(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5, const ParamPtr& p6, const ParamPtr& p7);
      State(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5, const ParamPtr& p6, const ParamPtr& p7, const ParamPtr& p8);
      State(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5, const ParamPtr& p6, const ParamPtr& p7, const ParamPtr& p8, const ParamPtr& p9);
      State(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5, const ParamPtr& p6, const ParamPtr& p7, const ParamPtr& p8, const ParamPtr& p9, const ParamPtr& p10);
    public:
      std::map<GLenum, ParamPtr> params;
      
      static StatePtr create();
      static StatePtr create(const ParamPtr& p1);
      static StatePtr create(const ParamPtr& p1, const ParamPtr& p2);
      static StatePtr create(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3);
      static StatePtr create(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4);
      static StatePtr create(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5);
      static StatePtr create(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5, const ParamPtr& p6);
      static StatePtr create(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5, const ParamPtr& p6, const ParamPtr& p7);
      static StatePtr create(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5, const ParamPtr& p6, const ParamPtr& p7, const ParamPtr& p8);
      static StatePtr create(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5, const ParamPtr& p6, const ParamPtr& p7, const ParamPtr& p8, const ParamPtr& p9);
      static StatePtr create(const ParamPtr& p1, const ParamPtr& p2, const ParamPtr& p3, const ParamPtr& p4, const ParamPtr& p5, const ParamPtr& p6, const ParamPtr& p7, const ParamPtr& p8, const ParamPtr& p9, const ParamPtr& p10);
      
      void param(const ParamPtr& inParam);
    };

  }
}

#endif
