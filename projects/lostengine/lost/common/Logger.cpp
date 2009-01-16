#include <string.h>
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

      static boost::mutex logMutex;

      void logMessage(const std::string& inLevel, const std::string& inLocation, const std::string& inMsg)
      {
        std::string t;
        logMutex.lock();
        std::cout << lost::platform::currentTimeFormat(t) << " \t " <<
                     inLevel                              << " \t " <<
                     inLocation                           << " \t " <<
                     inMsg                                << std::endl;	
        logMutex.unlock();
      }

		}
  }
}
