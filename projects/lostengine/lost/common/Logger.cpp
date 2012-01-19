#include <string.h>
#include <boost/thread/once.hpp>
#include <boost/thread/mutex.hpp>
#include "lost/common/Logger.h"
#include "lost/platform/Type.h"
#include "lost/platform/Time.h"
#include <iostream>

#ifdef ANDROID
extern "C" {
  #include <android/log.h>
}
#endif

namespace lost
{
  namespace common
  {
		namespace Logger
		{			

      string fileNameFromFullPath(const char* fullPath)
      {
        const char* p = strrchr(fullPath, '/'); // FIXME: this might fail on windows due to different separators, we might need to use something like boost::fs here
        if(p)
        {
          return string(p+1); // add 1 to pointer to remove leading '/'
        }
        else
        {
          return string(fullPath);
        }
      }

      boost::mutex* logMutex    = NULL;
      boost::once_flag initOnce = BOOST_ONCE_INIT;
      void initLogMutex()
      {
        if (!logMutex) logMutex = new boost::mutex;
      }
      
      void logMessage(const string& inLevel, const string& inLocation, const string& inMsg)
      {
        boost::call_once(initOnce, &initLogMutex);

        logMutex->lock();
#ifndef ANDROID
        std::cout << lost::platform::currentTimeFormat() << " \t " <<
                     inLevel                              << " \t " <<
                     inLocation                           << " \t " <<
                     inMsg                                << std::endl;
#else
        std::ostringstream os;
        os << lost::platform::currentTimeFormat() << " \t " <<
              inLevel                              << " \t " <<
              inLocation                           << " \t " <<
              inMsg;

        __android_log_print(ANDROID_LOG_INFO, "LostEngine", os.str().c_str());
#endif
        logMutex->unlock();
      }

		}
  }
}
