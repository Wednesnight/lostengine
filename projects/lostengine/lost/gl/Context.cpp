#include "lost/gl/gl.h"
#include "lost/gl/Context.h"
#include "lost/gl/Draw.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec2.h"
#include "lost/gl/Utils.h"
#include "lost/common/DisplayAttributes.h"


namespace lost
{
namespace gl
{

void Context::setState(const boost::shared_ptr<State>& newState)
{
#define SET_STATE_BOOL(which, target, source, attribute)\
    if (target->attribute != source->attribute)\
    {\
      target->attribute = source->attribute;\
      (target->attribute) ? glEnable(which) : glDisable(which);GLDEBUG;\
    }

  SET_STATE_BOOL(GL_ALPHA_TEST, state, newState, alphaTest);
  SET_STATE_BOOL(GL_DEPTH_TEST, state, newState, depthTest);
  SET_STATE_BOOL(GL_TEXTURE_2D, state, newState, texture2D);
  SET_STATE_BOOL(GL_BLEND, state, newState, blend);

  if((state->blendSrc != newState->blendSrc) || (state->blendDest != newState->blendDest))
  {
    state->blendSrc = newState->blendSrc;
    state->blendDest = newState->blendDest;
    glBlendFunc(state->blendSrc, state->blendDest);GLDEBUG;
  }

  if (state->clearColor != newState->clearColor)
  {
    state->clearColor = newState->clearColor;
    glClearColor(state->clearColor.r(), state->clearColor.g(), state->clearColor.b(), state->clearColor.a());GLDEBUG;
  }

#define SET_CLIENT_STATE_BOOL(which, target, source, attribute)\
    if (target->attribute != source->attribute)\
    {\
      target->attribute = source->attribute;\
      (target->attribute) ? glEnableClientState(which) : glDisableClientState(which);GLDEBUG;\
    }

  SET_CLIENT_STATE_BOOL(GL_NORMAL_ARRAY, state, newState, normalArray);
  SET_CLIENT_STATE_BOOL(GL_VERTEX_ARRAY, state, newState, vertexArray);
}


Context::Context()
{
  DOUT("lost::gl::Context::Context()");

  // initialize state
  pushState();
}

Context::~Context()
{
}

boost::shared_ptr<State> Context::newState()
{
  boost::shared_ptr<State> result(new State());
  
  glGetBooleanv(GL_ALPHA_TEST, (GLboolean*)&(result->alphaTest));GLDEBUG_THROW;
  glGetBooleanv(GL_DEPTH_TEST, (GLboolean*)&(result->depthTest));GLDEBUG_THROW;
  glGetBooleanv(GL_TEXTURE_2D, (GLboolean*)&(result->texture2D));GLDEBUG_THROW;

  glGetFloatv(GL_COLOR_CLEAR_VALUE, (GLfloat*)result->clearColor.fv);GLDEBUG_THROW;
  
  glGetBooleanv(GL_NORMAL_ARRAY, (GLboolean*)&(result->normalArray));GLDEBUG_THROW;
  glGetBooleanv(GL_VERTEX_ARRAY, (GLboolean*)&(result->vertexArray));GLDEBUG_THROW;

  GLfloat f = 0;
  glGetFloatv(GL_BLEND, &f);GLDEBUG_THROW;
  if(f != 0.0f)
    result->blend = true;
  else
    result->blend = false;
    
  f = 0;  
  glGetFloatv(GL_BLEND_SRC, &f);GLDEBUG_THROW;
  result->blendSrc = static_cast<GLenum>(f);

  f = 0;
  glGetFloatv(GL_BLEND_DST, &f);GLDEBUG_THROW;
  result->blendDest = static_cast<GLenum>(f);
    
  return result;
}
      
boost::shared_ptr<State> Context::copyState()
{
  return boost::shared_ptr<State>(new State(*(state.get())));
}
      
void Context::pushState()
{
  pushState(state);
}

void Context::pushState(const boost::shared_ptr<State>& inState)
{
  if (state) stateStack.push_back(state);
  state = newState();
  if (inState) setState(inState);
}

void Context::popState()
{
  if (stateStack.size() > 0)
  {
    setState(stateStack.back());
    stateStack.pop_back();
  }
}

void Context::clear(GLbitfield flags)
{
  glClear(flags);GLDEBUG;
}

void Context::set2DProjection(const lost::math::Vec2& offset, const lost::math::Vec2& dimension)
{
  lost::gl::utils::set2DProjection(offset, dimension);
}
      
void Context::set3DProjection(const lost::common::DisplayAttributes& attributes,
                            const lost::math::Vec3& eye,
                            const lost::math::Vec3& at,
                            const lost::math::Vec3& up,
                            const float& fovy,
                            const lost::math::Vec2& depth)
{
  lost::gl::utils::set3DProjection(attributes.width, attributes.height, eye, at, up, fovy, depth.min, depth.max);
}

boost::shared_ptr<State>& Context::defaultVertexState()
{
  static boost::shared_ptr<State> newState;
  if (!newState)
  {
    newState = copyState();
    newState->vertexArray = true;
  }
  return newState;
}

void Context::drawLine(const lost::math::Vec2& start, const lost::math::Vec2& end)
{
  pushState(defaultVertexState());
  lost::gl::drawLine(start, end);
  popState();
}

void Context::drawLine(const lost::math::Vec3& start, const lost::math::Vec3& end)
{
  pushState(defaultVertexState());
  lost::gl::drawLine(start, end);
  popState();
}
        
}
}

