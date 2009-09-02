#include "lost/application/Application.h"
#include "lost/application/Tasklet.h"

int main(int argn, char** args)
{
  return lost::application::runTasklet(new lost::application::Tasklet);
}
