// platform specific code, forwarded in Platform.h
// LINUX

#include <string>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include "boost/filesystem.hpp"

namespace lost
{
  namespace platform
  {

    // returns current time as string (e.g. "2007/11/26 23:30:37")
    std::string& currentTimeFormat( std::string& sTime )
    {
      char   timeformat[20];
      time_t timestamp;

      timestamp = time( &timestamp );
      if (strftime( timeformat, 20, "%Y/%m/%d %H:%M:%S", localtime( &timestamp ) ) > 0)
        sTime = timeformat;

      return sTime;
    }

    // returns current time in microseconds
    double currentTimeMicroSeconds()
    {
      struct timeval tv;

      gettimeofday(&tv, NULL);
      return ((double)tv.tv_sec)*1000000.0 + (double)tv.tv_usec;
    }

    std::string getApplicationDirectory()
    {
      boost::filesystem::path path = "/proc";
      struct stat info;
      if (0 != stat(path.string().c_str(), &info) || !S_ISDIR(info.st_mode)) {
        // There's no /proc filesystem, we can't find out a lot about our
        // application.
        throw std::runtime_error("Could not find /proc filesystem!");
      }


      // Read the exe link in the /proc filesystem
      ostringstream os;
      os << getpid();
      path /= os.str();
      path /= "exe";

      // There's no limit to how long a path in Linux can be, but let's assume
      // it won't exceed 2k characters.
      char pathbuf[2048];

      ssize_t pathsize = readlink(path.string().c_str(), pathbuf,
          sizeof(pathbuf));

      if (-1 == pathsize) {
        throw std::runtime_error("Could not determine application path!");
      }

      path = std::string(pathbuf, pathsize);
      return path.branch_path().string();
    }

    // TODO: getApplicationFilename() not implemented
    std::string getApplicationFilename( bool excludeExtension = false )
    {
      std::string result;
      return result;
    }

    // TODO: buildResourcePath() not implemented
    std::string buildResourcePath( const std::string& filename )
    {
      std::string result( filename );
      return result;
    }

    std::string getResourcePath()
    {
      std::string result;
      return result;
    }

    // TODO: buildUserDataPath() not implemented
    std::string buildUserDataPath( const std::string& filename )
    {
      std::string result( filename );
      return result;
    }

  }
}
