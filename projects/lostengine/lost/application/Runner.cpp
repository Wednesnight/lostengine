/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/application/Runner.h"
#include "lost/application/Application.h"
#include "lost/application/Tasklet.h"
#include "lost/common/Logger.h"
#include "lost/resource/FilesystemRepository.h"
#include "lost/platform/Platform.h"

#ifdef ANDROID
#include "lost/resource/AndroidAssetRepository.h"
extern "C" {
  #include <jni.h>
  #include <android_native_app_glue.h>
  
  extern struct android_app* getAndroidApp();
}
#endif

namespace lost
{
  namespace application
  {
    int runResourceTasklet(int argc, char *argv[], const string& relativePathToTaskletInResourceDir)
    {
#ifndef ANDROID
      resource::LoaderPtr loader = resource::DefaultLoader::create();
      lost::fs::Path taskletPath(platform::getResourcePath());
      taskletPath /= relativePathToTaskletInResourceDir;
      resource::RepositoryPtr taskletDir(new resource::FilesystemRepository(taskletPath));
#else
      resource::LoaderPtr loader = resource::Loader::create();
      resource::RepositoryPtr taskletDir(new resource::AndroidAssetRepository(
          getAndroidApp()->activity->assetManager, relativePathToTaskletInResourceDir));
#endif
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
        static ApplicationPtr app = Application::getInstance(t);
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
