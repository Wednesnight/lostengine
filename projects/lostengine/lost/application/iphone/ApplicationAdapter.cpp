#include "lost/application/ApplicationAdapter.h"
#include "lost/application/iphone/GLContext.h"

struct ApplicationAdapterState
{
  GLContext glcontext;
};


ApplicationAdapter::ApplicationAdapter(lost::event::EventDispatcher* inTarget)
:target(inTarget)
{
  state.reset(new ApplicationAdapterState);
}

ApplicationAdapter::~ApplicationAdapter()
{
}

void ApplicationAdapter::init(const lost::common::DisplayAttributes& displayAttributes)
{
  state->glcontext.init(displayAttributes);
}

void ApplicationAdapter::run()
{
}

void ApplicationAdapter::swapBuffers()
{
}

void ApplicationAdapter::quit()
{
}

void ApplicationAdapter::terminate()
{
}

