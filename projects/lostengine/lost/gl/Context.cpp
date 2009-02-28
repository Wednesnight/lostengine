#include "lost/gl/Context.h"

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
    }
    
    Context::~Context()
    {
      finalize();
    }

    void Context::apply(const SharedState& newState)
    {
      for (std::map<GLenum, SharedParam>::iterator idx = newState->params.begin(); idx != newState->params.end(); ++idx)
      {
        if (currentState->params.find(idx->first) == currentState->params.end() ||
            currentState->params[idx->first].get() != idx->second.get())
        {
          currentState->params[idx->first] = idx->second;
          idx->second->set();
        }
      }
    }
    
    void Context::pushState(const SharedState& newState)
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
        SharedState state = stateStack.back();
        apply(state);
        stateStack.pop_back();
      }
    }

  }
}
