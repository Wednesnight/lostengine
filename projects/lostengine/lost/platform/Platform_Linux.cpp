// platform specific code, forwarded in Platform.h
// LINUX

#include "lost/platform/Platform.h"
#include <string>
#include <stdexcept>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <iostream>
#include <sstream>
#include <stdio.h>

#include <sys/prctl.h>

using namespace std;

namespace lost
{
  namespace platform
  {

    // returns current time as string (e.g. "2007/11/26 23:30:37")
    string currentTimeFormat()
    {
      char   timeformat[20];
      time_t timestamp;

      timestamp = time( &timestamp );
      strftime(timeformat, 20, "%Y/%m/%d %H:%M:%S", localtime( &timestamp ));
      return timeformat;
    }

    // returns current time in microseconds
    double currentTimeMicroSeconds()
    {
      struct timeval tv;

      gettimeofday(&tv, NULL);
      return ((double)tv.tv_sec)*1000000.0 + (double)tv.tv_usec;
    }

    lost::fs::Path getApplicationFilename(bool excludeExtension = false)
    {
      lost::fs::Path path = "/proc";
      struct stat info;
      if (0 != stat(path.string().c_str(), &info) || !S_ISDIR(info.st_mode)) {
        // There's no /proc filesystem, we can't find out a lot about our
        // application.
        throw std::runtime_error("Could not find /proc filesystem!");
      }


      // Read the exe link in the /proc filesystem
      std::ostringstream os;
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

      return string(pathbuf, pathsize);
    }

    lost::fs::Path getApplicationDirectory()
    {
      return getApplicationFilename().branch_path();
    }

    lost::fs::Path buildResourcePath( const string& filename )
    {
      lost::fs::Path result = getResourcePath() / filename;

      FILE *l_filecheck = fopen(result.string().c_str(), "r");
      if (l_filecheck != NULL) fclose(l_filecheck);
        else throw runtime_error( "Couldn't find resource '"+ result.string() +"', does it exist in your resources directory? Is the spelling correct?" );

      return result;
    }

    lost::fs::Path getResourcePath()
    {
      return getApplicationDirectory();
    }

    lost::fs::Path getUserDataPath() {
      lost::fs::Path result;
#ifdef ANDROID
      struct passwd* pwdptr = getpwuid(getuid());
      if (pwdptr != NULL) {
	result = pwdptr->pw_dir;
      }
#else
      struct passwd pd;
      struct passwd* pwdptr=&pd;
      struct passwd* tempPwdPtr;
      char pwdbuffer[200];
      int  pwdlinelen = sizeof(pwdbuffer);

      if ((getpwuid_r(getuid(), pwdptr, pwdbuffer, pwdlinelen, &tempPwdPtr)) == 0) {
	result = pd.pw_dir;
      }
#endif
      return result;
    }

    lost::fs::Path buildUserDataPath( const string& filename )
    {
      lost::fs::Path result = getUserDataPath() / filename;

      FILE *l_filecheck = fopen(result.string().c_str(), "r");
      if (l_filecheck != NULL) fclose(l_filecheck);
        else throw runtime_error( "Couldn't find resource '"+ result.string() +"', does it exist in your resources directory? Is the spelling correct?" );

      return result;
    }

    void setThreadName(const string& name) {
      prctl(PR_SET_NAME, name.c_str(), 0, 0, 0);
    }

  }
}
