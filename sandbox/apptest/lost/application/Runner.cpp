#include "lost/application/Runner.h"
#include "lost/application/Application.h"
#include "lost/application/Tasklet.h"
#include "lost/common/Logger.h"
#include "lost/resource/FilesystemRepository.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/platform/Platform.h"
#include <boost/filesystem.hpp>

namespace lost
{
  namespace application
  {
    int runResourceTasklet(int argc, char *argv[], const eastl::string& relativePathToTaskletInResourceDir)
    {
      resource::LoaderPtr loader = resource::DefaultLoader::create();
      boost::filesystem::path taskletPath(platform::getResourcePath());
      taskletPath /= relativePathToTaskletInResourceDir.c_str();
      resource::RepositoryPtr taskletDir(new resource::FilesystemRepository(taskletPath));
      loader->addRepository(taskletDir);
      Tasklet* tasklet = new Tasklet(loader);
      return runTasklet(argc, argv, tasklet);
    }
        

    int runTasklet(int argc, char *argv[], Tasklet* t)
    {
      int result = EXIT_SUCCESS;
      try
      {
        static ApplicationPtr app = Application::getInstance();
        app->startTasklet(t);
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