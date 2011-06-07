// platform specific code, forwarded in Platform.h
// MAC

#include <CoreFoundation/CoreFoundation.h>
#include <string>
#include <stdexcept>
#include <sys/time.h>
#include "boost/filesystem.hpp"
using namespace std;

namespace lost
{
  namespace platform
  {

    // returns current time as string (e.g. "2007/11/26 23:30:37")
    std::string currentTimeFormat()
    {
      const size_t bufsize = 30;
      char   timeformat[bufsize];

      struct timeval tv;
      gettimeofday(&tv, NULL);

      strftime( timeformat, bufsize, "%Y/%m/%d %H:%M:%S", localtime(&tv.tv_sec));
      timeformat[19] = '.';
      timeformat[20] = '%';
      timeformat[21] = '0';
      timeformat[22] = '3';
      timeformat[23] = 'd';
      timeformat[24] = 0;
      snprintf(timeformat, bufsize, timeformat, tv.tv_usec/1000);

      return timeformat;
    }

    // returns current time in microseconds
    double currentTimeMicroSeconds()
    {
      struct timeval tv;

      gettimeofday(&tv, NULL);
      return ((double)tv.tv_sec)*1000000.0 + (double)tv.tv_usec;
    }

    // TODO: getApplicationDirectory() not implemented
    std::string getApplicationDirectory()
    {
      std::string result;
      return result;
    }

    boost::filesystem::path getResourcePath()
    {
      CFBundleRef mainBundle = CFBundleGetMainBundle();
      CFURLRef resourceDirURL = CFBundleCopyResourcesDirectoryURL(mainBundle);

      if(!resourceDirURL)
      {
        throw runtime_error( "Couldn't find resource directory, does it exist?" );
      }

      const unsigned long bufsize = 4096;
      UInt8 buffer[bufsize];
      buffer[0] = 0;
      Boolean convresult = CFURLGetFileSystemRepresentation( resourceDirURL,
                                                            true,
                                                            buffer,
                                                            bufsize );
      
      if(!convresult)
      {
        throw runtime_error("couldn't convert CFURL to path for resource dir");
      }
      
      string result(reinterpret_cast<char*>(buffer));
      CFRelease(resourceDirURL);
      
      return boost::filesystem::path(result);
    }
  }
}
