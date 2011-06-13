#include <string.h>
#include <boost/thread/once.hpp>
#include <boost/thread/mutex.hpp>
#include "lost/common/Logger.h"
#include "lost/platform/Type.h"
#include "lost/platform/Time.h"

namespace lost
{
  namespace common
  {
		namespace Logger
		{			

      eastl::string fileNameFromFullPath(const char* fullPath)
      {
        boost::filesystem::path p(fullPath);
        return eastl::string(p.filename().native().c_str());
      }

      boost::mutex* logMutex    = NULL;
      boost::once_flag initOnce = BOOST_ONCE_INIT;
      void initLogMutex()
      {
        if (!logMutex) logMutex = new boost::mutex;
      }
      
      void logMessage(const char* inLevel, const char* inLocation, const char* inMsg)
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
