#include "lost/application/glfw/LostApplicationImpl.h"
#include "lost/common/DisplayAttributes.h"

using namespace lost::event;
using namespace lost;
using namespace lost::application;
using namespace boost;
using namespace lost::common;
using namespace std;

LostApplicationImpl::LostApplicationImpl(EventDispatcher* target)
: adapter(target) // instantiate impl AFTER appInstance pointer was set!
{
}
