#include "lost/application/Tasklet.h"
#include "lost/lua/State.h"

namespace lost
{
namespace application
{

Tasklet::Tasklet(resource::LoaderPtr inLoader)
{
  lua.reset(new lua::State);
}

Tasklet::~Tasklet()
{
}

}
}