#include <string.h>
#include <boost/thread/once.hpp>
#include <boost/thread/mutex.hpp>
#include "lost/common/Logger.h"
#include "lost/platform/Platform.h"

namespace lost
{
  namespace common
  {
		namespace Logger
		{			

      std::string fileNameFromFullPath(const char* fullPath)
      {
        const char* p = strrchr(fullPath, '/'); // FIXME: this might fail on windows due to different separators, we might need to use something like boost::fs here
        if(p)
        {
          return std::string(p+1); // add 1 to pointer to remove leading '/'
        }
        else
        {
          return std::string(fullPath);
        }
      }

      boost::mutex* logMutex    = NULL;
      boost::once_flag initOnce = BOOST_ONCE_INIT;
      void initLogMutex()
      {
        if (!logMutex) logMutex = new boost::mutex;
      }
      
      void logMessage(const std::string& inLevel, const std::string& inLocation, const std::string& inMsg)
      {
        boost::call_once(initOnce, &initLogMutex);

        logMutex->lock();
        std::cout << lost::platform::currentTimeFormat() << " \t " <<
                     inLevel                              << " \t " <<
                     inLocation                           << " \t " <<
                     inMsg                                << std::endl;	
        logMutex->unlock();
      }

		}
  }
}
