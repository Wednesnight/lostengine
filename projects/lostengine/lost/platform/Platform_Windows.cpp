// platform specific code, forwarded in Platform.h
// WINDOWS

#include <stdexcept>
#include <shlobj.h>
#include <shlwapi.h>
#include <iostream>
#include <time.h>

namespace lost
{
  namespace platform
  {

//*** HELPER FUNCTIONS

    void splitApplicationPath( string* drive,    string* directory,
                               string* filename, string* extension )
    {
      char   l_executable[MAX_PATH];
      char   l_drive[_MAX_DRIVE];
      char   l_directory[_MAX_DIR];
      char   l_filename[_MAX_FNAME];
      char   l_extension[_MAX_EXT];

      if (GetModuleFileNameA( 0, l_executable, MAX_PATH ))
      {
        _splitpath_s( l_executable, l_drive, l_directory, l_filename, l_extension );
        if (drive)     *drive     = reinterpret_cast<char*>(l_drive);
        if (directory) *directory = reinterpret_cast<char*>(l_directory);
        if (filename)  *filename  = reinterpret_cast<char*>(l_filename);
        if (extension) *extension = reinterpret_cast<char*>(l_extension);
      }
    }

//***

    // returns current time as string (e.g. "2007/11/26 23:30:37")
    string currentTimeFormat()
    {
      char   timeformat[20];
      time_t timestamp;
      tm     localtimestamp;

      timestamp = time( &timestamp );
      if (localtime_s(&localtimestamp, &timestamp) == 0 &&
          strftime( timeformat, 20, "%Y/%m/%d %H:%M:%S", &localtimestamp ) > 0)
      {
        return timeformat;
      }

      return "";
    }

    // returns current time in microseconds
    double currentTimeMicroSeconds()
    {
      FILETIME      filetime;
      LARGE_INTEGER timevalue;

      GetSystemTimeAsFileTime( &filetime );
      timevalue.LowPart  = filetime.dwLowDateTime;
      timevalue.HighPart = filetime.dwHighDateTime;

      return (double)(timevalue.QuadPart / 10);
    }

    lost::fs::Path getApplicationDirectory()
    {
      string result;
      string l_drive;
      string l_directory;
      char   l_path[MAX_PATH];

      splitApplicationPath( &l_drive, &l_directory, NULL, NULL );
      _makepath_s( l_path, l_drive.c_str(), l_directory.c_str(), NULL, NULL );
      PathAddBackslashA( l_path );
      result = reinterpret_cast<char*>(l_path);

      return lost::fs::Path(result);
    }

    lost::fs::Path getApplicationFilename( bool excludeExtension )
    {
      string l_filename;
      string l_extension;

      splitApplicationPath( NULL, NULL, &l_filename, &l_extension );
      return lost::fs::Path(
        (!excludeExtension && !l_extension.empty()) ? l_filename.append( l_extension ) : l_filename);
    }

    lost::fs::Path buildResourcePath( const string& filename )
    {
      lost::fs::Path result(getApplicationDirectory() / filename);

      FILE *l_filecheck;
      if (fopen_s(&l_filecheck, result.string().c_str(), "r") == 0) fclose( l_filecheck );
        else throw std::runtime_error("Couldn't find resource '"+ result.string() +"', does it exist in your resources directory? Is the spelling correct?");

      return result;
    }

    lost::fs::Path getResourcePath()
    {
      return getApplicationDirectory();
    }

    lost::fs::Path getUserDataPath()
    {
      lost::fs::Path result;
      char l_path[MAX_PATH];

      if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, l_path)))
      {
        result = reinterpret_cast<char*>(l_path);
      }
      return result;
    }

    lost::fs::Path buildUserDataPath( const string& filename )
    {
      lost::fs::Path result = getUserDataPath() / filename;

      FILE *l_filecheck;
      if (fopen_s(&l_filecheck, result.string().c_str(), "r") == 0) fclose( l_filecheck );
        else throw std::runtime_error("Couldn't find resource '"+ result.string() +"', does it exist in your user data directory? Is the spelling correct?");

      return result;
    }

    /**
      see: http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
      */
    //
    // Usage: SetThreadName (-1, "MainThread");
    //
    const DWORD MS_VC_EXCEPTION = 0x406D1388;

    #pragma pack(push,8)
    typedef struct tagTHREADNAME_INFO
    {
       DWORD dwType; // Must be 0x1000.
       LPCSTR szName; // Pointer to name (in user addr space).
       DWORD dwThreadID; // Thread ID (-1=caller thread).
       DWORD dwFlags; // Reserved for future use, must be zero.
    } THREADNAME_INFO;
    #pragma pack(pop)

    void setThreadName(const string& name)
    {
       THREADNAME_INFO info;
       info.dwType = 0x1000;
       info.szName = name.c_str();
       info.dwThreadID = -1;
       info.dwFlags = 0;

       __try
       {
          RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
       }
       __except(EXCEPTION_EXECUTE_HANDLER)
       {
       }
    }

  }
}
