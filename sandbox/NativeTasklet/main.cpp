#include "lost/application/Runner.h"
#include "NativeTasklet.h"

int main(int argn, char** args)
{
  return lost::application::runTasklet(new NativeTasklet);
}
