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
        std::cout << lost::platform::currentTimeFormat()  << " \t " <<
                     lost::platform::getThreadName()      << " \t " << 
                     inLevel                              << " \t " <<
                     inLocation                           << " \t " <<
                     inMsg                                << std::endl;
#else
        std::ostringstream os;
        os << lost::platform::currentTimeFormat() << " \t " <<
              inLevel                             << " \t " <<
              inLocation                          << " \t " <<
              inMsg;

        __android_log_print(ANDROID_LOG_INFO, lost::platform::getThreadName().c_str(), os.str().c_str());
#endif
        logMutex->unlock();
      }

		}
  }
}
