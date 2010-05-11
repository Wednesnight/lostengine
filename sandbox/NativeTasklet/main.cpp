#include "lost/application/Runner.h"
#include "NativeTasklet.h"

int main(int argc, char *argv[])
{
  return lost::application::runTasklet(argc, argv, new NativeTasklet);
}
