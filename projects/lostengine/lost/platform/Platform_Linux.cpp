// platform specific code, forwarded in Platform.h
// LINUX

#include <string>
#include <sys/time.h>

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
      struct timezone tz;
      struct timeval tv;

      gettimeofday(&tv, &tz);
      return ((double)tv.tv_sec)*1000000.0 + (double)tv.tv_usec;
    }

    // TODO: getApplicationDirectory() not implemented
    std::string getApplicationDirectory()
    {
      std::string result;
      return result;
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
