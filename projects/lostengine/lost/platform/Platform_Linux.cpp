// platform specific code, forwarded in Platform.h
// LINUX

#include <string>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>

#include "boost/filesystem.hpp"
#include "boost/lexical_cast.hpp"

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

    // TODO: Needs some thought as to defaults/ LSB-behaviour, but kind of
    // works.
    std::string getApplicationDirectory()
    {
      // The best default thing to do is to assume LSB-paths, i.e.
      // /usr/share/<appname>.
      // XXX We'd need the Application to be a singleton  object and to have a
      //     name, though - so we'll default to the current directory.
      std::string default_path = std::string("./");

      boost::filesystem::path path = "/proc";
      struct stat info;
      if (0 != stat(path.string().c_str(), &info) || !S_ISDIR(info.st_mode)) {
        // There's no /proc filesystem, we can't find out a lot about our
        // application.
        std::cerr << "Can't find /proc filesystem, defaulting to '" << default_path << "'.";
        return default_path;
      }


      // Read the exe link in the /proc filesystem
      path /= boost::lexical_cast<std::string>(getpid());
      path /= "exe";

      // There's no limit to how long a path in Linux can be, but let's assume
      // it won't exceed 2k characters.
      char pathbuf[2048];

      ssize_t pathsize = readlink(path.string().c_str(), pathbuf,
          sizeof(pathbuf));

      if (-1 == pathsize) {
        std::cerr << "Could not determine application path, defaulting to '" << default_path << "'.";
        return default_path;
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
