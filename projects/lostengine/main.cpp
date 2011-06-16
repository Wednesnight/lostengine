#include "lost/application/Runner.h"

int main(int argn, char** args)
{
  return lost::application::runResourceTasklet(argn, args, "Launcher.tasklet");
}
