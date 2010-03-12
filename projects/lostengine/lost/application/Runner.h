#ifndef LOST_APPLICATION_RUNNER_H
#define LOST_APPLICATION_RUNNER_H

namespace lost
{
  namespace application
  {

    struct Tasklet;

    int runTasklet(int argc, char *argv[]);
    int runTasklet(int argc, char *argv[], Tasklet* t);
      
  }
}

#endif
