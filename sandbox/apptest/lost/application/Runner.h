#ifndef LOST_APPLICATION_RUNNER_H
#define LOST_APPLICATION_RUNNER_H

#include "lost/application/forward.h"

namespace lost
{
  namespace application
  {
    int runResourceTasklet(int argc, char *argv[], const eastl::string& relativePathToTaskletInResourceDir);
    int runTasklet(int argc, char *argv[], Tasklet* t);      
  }
}

#endif
