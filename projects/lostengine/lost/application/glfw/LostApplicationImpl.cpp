#include "lost/application/glfw/LostApplicationImpl.h"
#include "lost/common/DisplayAttributes.h"

using namespace lost::event;
using namespace lost;
using namespace lost::application;
using namespace boost;
using namespace lost::common;
using namespace std;

LostApplicationImpl::LostApplicationImpl(EventDispatcher* target)
: adapter(target), delegate(target) // instantiate impl AFTER appInstance pointer was set!
{
  initialised = false;
  running = false;// the main loop is not running yet
}
    
void LostApplicationImpl::init(const common::DisplayAttributes& displayAttributes)
{  
  // init display/adapter so all other code is safe from here on because an OpenGL context now exists
  adapter.init(displayAttributes);
  
}
