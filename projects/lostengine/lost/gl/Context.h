#ifndef LOST_GL_CONTEXT_H
#define LOST_GL_CONTEXT_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include "lost/gl/State.h"

namespace lost { namespace math{ struct Vec3;}}
namespace lost { namespace math{ struct Vec2;}}
namespace lost { namespace common{ struct DisplayAttributes;}}
namespace lost { namespace gl{ struct State;}}

namespace lost
{
  namespace gl
  {

    struct Context
    {
    private:
      std::vector<boost::shared_ptr<State> > stateStack;

      void setState(const boost::shared_ptr<State>& newState);

    public:
      boost::shared_ptr<State> state;

      Context();
      virtual ~Context();

      static boost::shared_ptr<State> newState();
      boost::shared_ptr<State> copyState();
      void pushState();
      void pushState(const boost::shared_ptr<State>& inState);
      void popState();
      void clear(GLbitfield flags);
      void set2DProjection(const lost::math::Vec2& offset, const lost::math::Vec2& dimension);
      void set3DProjection(const lost::common::DisplayAttributes& attributes,
                                  const lost::math::Vec3& eye,
                                  const lost::math::Vec3& at,
                                  const lost::math::Vec3& up,
                                  const float& fovy,
                                  const lost::math::Vec2& depth);      
      boost::shared_ptr<State>& defaultVertexState();
      void drawLine(const lost::math::Vec2& start, const lost::math::Vec2& end);
      void drawLine(const lost::math::Vec3& start, const lost::math::Vec3& end);
    };
        
  }
}

#endif
