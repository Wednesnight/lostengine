#include "lost/resource/forward.h"

namespace lost
{
namespace application
{

struct Tasklet
{
  Tasklet(resource::LoaderPtr inLoader);
  virtual ~Tasklet();
  
  
  resource::LoaderPtr loader;
};

}
}