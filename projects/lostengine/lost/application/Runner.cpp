#include "lost/application/Runner.h"
#include "lost/application/Application.h"
#include "lost/application/Tasklet.h"
#include "lost/common/Logger.h"
#include "lost/resource/FilesystemRepository.h"
#include "lost/platform/Platform.h"
#include <boost/filesystem.hpp>

namespace lost
{
  namespace application
  {
    int runResourceTasklet(int argc, char *argv[], const std::string& relativePathToTaskletInResourceDir)
    {
      resource::LoaderPtr loader = resource::DefaultLoader::create();
      boost::filesystem::path taskletPath(platform::getResourcePath());
      taskletPath /= relativePathToTaskletInResourceDir;
      resource::RepositoryPtr taskletDir(new resource::FilesystemRepository(taskletPath));
      loader->addRepository(taskletDir);
      Tasklet* tasklet = new Tasklet(loader);
      return runTasklet(argc, argv, tasklet);
    }
    
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