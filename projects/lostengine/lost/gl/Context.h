#ifndef LOST_GL_CONTEXT_H
#define LOST_GL_CONTEXT_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include "lost/gl/gl.h"
#include "lost/gl/Draw.h"
#include "lost/gl/State.h"
#include "lost/common/Logger.h"
#include "lost/math/Vec3.h"

namespace lost
{
  namespace gl
  {

    struct Context
    {
    private:
      std::vector<boost::shared_ptr<State> > stateStack;

      void setState(const boost::shared_ptr<State>& newState)
      {
#define SET_STATE_ATTRIBUTE(which, target, source, attribute)\
          if (target->attribute != source->attribute)\
          {\
            target->attribute = source->attribute;\
            (target->attribute) ? glEnable(which) : glDisable(which);GLDEBUG;\
          }

        SET_STATE_ATTRIBUTE(GL_ALPHA_TEST, state, newState, alphaTest);
        SET_STATE_ATTRIBUTE(GL_DEPTH_TEST, state, newState, depthTest);
        SET_STATE_ATTRIBUTE(GL_TEXTURE_2D, state, newState, texture2D);

#define SET_STATE_CLIENT_ATTRIBUTE(which, target, source, attribute)\
          if (target->attribute != source->attribute)\
          {\
            target->attribute = source->attribute;\
            (target->attribute) ? glEnableClientState(which) : glDisableClientState(which);GLDEBUG;\
          }

        SET_STATE_CLIENT_ATTRIBUTE(GL_NORMAL_ARRAY, state, newState, normalArray);
        SET_STATE_CLIENT_ATTRIBUTE(GL_VERTEX_ARRAY, state, newState, vertexArray);
      }

    public:
      boost::shared_ptr<State> state;

      Context()
      {
        DOUT("lost::gl::Context::Context()");

        // initialize state
        pushState();
      };

      virtual ~Context()
      {
      };

      static inline boost::shared_ptr<State> newState()
      {
        boost::shared_ptr<State> result(new State());
        
        glGetBooleanv(GL_ALPHA_TEST, (GLboolean*)&result->alphaTest);
        glGetBooleanv(GL_DEPTH_TEST, (GLboolean*)&result->depthTest);
        glGetBooleanv(GL_TEXTURE_2D, (GLboolean*)&result->texture2D);
        
        glGetBooleanv(GL_NORMAL_ARRAY, (GLboolean*)&result->normalArray);
        glGetBooleanv(GL_VERTEX_ARRAY, (GLboolean*)&result->vertexArray);
        
        return result;
      }
      
      inline boost::shared_ptr<State> copyState()
      {
        return boost::shared_ptr<State>(new State(*(state.get())));
      }
      
      void pushState()
      {
        pushState(state);
      }

      void pushState(const boost::shared_ptr<State>& inState)
      {
        if (state) stateStack.push_back(state);
        state = newState();
        if (inState) setState(inState);
      }

      void popState()
      {
        if (stateStack.size() > 0)
        {
          setState(stateStack.back());
          stateStack.pop_back();
        }
      }

      inline boost::shared_ptr<State>& defaultVertexState()
      {
        static boost::shared_ptr<State> newState;
        if (!newState)
        {
          newState = copyState();
          newState->vertexArray = true;
        }
        return newState;
      }

      template <typename Vec>
      inline void drawLine(const Vec& start, const Vec& end)
      {
        pushState(defaultVertexState());
        lost::gl::drawLine(start, end);
        popState();
      }

      void drawLine(const lost::math::Vec2& start, const lost::math::Vec2& end)
      {
        drawLine<lost::math::Vec2>(start, end);
      }

      void drawLine(const lost::math::Vec3& start, const lost::math::Vec3& end)
      {
        drawLine<lost::math::Vec3>(start, end);
      }
    };
        
  }
}

#endif
