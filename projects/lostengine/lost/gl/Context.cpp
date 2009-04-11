#include <boost/shared_ptr.hpp>
#include "lost/gl/Context.h"
#include "lost/lgl/lgl.h"

namespace lost
{
  namespace gl
  {

    Context::Context()
    {
      initialize();
      
      // push back initial state
      currentState = State::create();
      currentState->param(AlphaTest::create());
      currentState->param(DepthTest::create());
      currentState->param(Texture2D::create());
      currentState->param(Blend::create());
      currentState->param(Scissor::create());
      currentState->param(PointSmooth::create());
      currentState->param(LineSmooth::create());
      currentState->param(NormalArray::create());
      currentState->param(VertexArray::create());
      currentState->param(TextureArray::create());
      currentState->param(ClearColor::create());
      currentState->param(BlendFunc::create());
      currentState->param(ScissorBox::create());
      pushState(currentState);

      // this is only true on desktop systems. 
      // on systems like the iPhone, where you have to create the default buffer yourself, you
      // have to set the default in the context, after creation
      defaultFrameBuffer = 0; 
    }
    
    Context::~Context()
    {
      finalize();
    }

    void Context::bindFramebuffer(GLuint fbo)
    {
      lglBindFramebuffer(LGL_FRAMEBUFFER, fbo);GLDEBUG_THROW;        
    }
    
    void Context::bindDefaultFramebuffer()
    {
        bindFramebuffer(defaultFrameBuffer);
    }

    void Context::defaultFramebuffer(GLuint fbo)
    {
        defaultFrameBuffer = fbo;
    }
    
    GLuint Context::defaultFramebuffer()
    {
        return defaultFrameBuffer;
    }


    void Context::apply(const StatePtr& newState)
    {
      for (std::map<GLenum, ParamPtr>::iterator idx = newState->params.begin(); idx != newState->params.end(); ++idx)
      {
        if (currentState->params.find(idx->first) == currentState->params.end() ||
            currentState->params[idx->first].get() != idx->second.get())
        {
          currentState->params[idx->first] = idx->second;
          idx->second->set();
        }
      }
    }
    
    void Context::pushState(const StatePtr& newState)
    {
      if (stateStack.size() > 0)
      {
        apply(newState);
      }
      stateStack.push_back(newState);
    }

    void Context::popState()
    {
      if (stateStack.size() > 1)
      {
        StatePtr state = stateStack.back();
        apply(state);
        stateStack.pop_back();
      }
    }

  }
}
