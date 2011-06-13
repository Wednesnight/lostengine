#include "lost/resource/forward.h"
#include "lost/lua/forward.h"

namespace lost
{
namespace application
{

struct Tasklet
{
  Tasklet(resource::LoaderPtr inLoader);
  virtual ~Tasklet();
  
  lua::StatePtr lua;
  resource::LoaderPtr loader;
};

}
}