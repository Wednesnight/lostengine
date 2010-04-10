#include "lost/application/Runner.h"
#include <string>

int main(int argn, char** args)
{
  return lost::application::runResourceTasklet(argn, args, "Launcher.tasklet");
}
