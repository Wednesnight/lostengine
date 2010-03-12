#include "lost/application/Runner.h"
#include "lost/application/Application.h"
#include "lost/application/Tasklet.h"
#include "lost/common/Logger.h"

namespace lost
{
  namespace application
  {

    int runTasklet(int argc, char *argv[])
    {
      return runTasklet(argc, argv, new Tasklet);
    }
    

    int runTasklet(int argc, char *argv[], Tasklet* t)
    {
      int result = EXIT_SUCCESS;
      try
      {
        static ApplicationPtr app = Application::create(t);
        app->run(argc, argv);      
      }
      catch (std::exception& e)
      {
        EOUT("exception: " << e.what());
        result = EXIT_FAILURE;
      }        
      return result;
    }

  }
}