#include "lost/application/SingleThreadedTasklet.h"

namespace lost
{
namespace application
{

SingleThreadedTasklet::SingleThreadedTasklet(lost::resource::LoaderPtr inLoader)
: Tasklet(inLoader)
{
}

SingleThreadedTasklet::~SingleThreadedTasklet()
{
}


}
}