// platform specific code, forwarded in Platform.h
// WINDOWS

#include <string>
#include <stdexcept>
#include <shlobj.h>
#include <shlwapi.h>
#include <iostream>
#include <time.h>

using namespace std;

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

    string getApplicationDirectory()
    {
      string result;
      string l_drive;
      string l_directory;
      char   l_path[MAX_PATH];

      splitApplicationPath( &l_drive, &l_directory, NULL, NULL );
      _makepath_s( l_path, l_drive.c_str(), l_directory.c_str(), NULL, NULL );
      PathAddBackslashA( l_path );
      result = reinterpret_cast<char*>(l_path);

      return result;
    }

    string getApplicationFilename( bool excludeExtension )
    {
      string l_filename;
      string l_extension;

      splitApplicationPath( NULL, NULL, &l_filename, &l_extension );
      return (!excludeExtension && !l_extension.empty()) ? l_filename.append( l_extension ) : l_filename;
    }

    string buildResourcePath( const string& filename )
    {
      string result( getApplicationDirectory().append( filename ) );

      FILE *l_filecheck;
      if (fopen_s(&l_filecheck, result.c_str(), "r") == 0) fclose( l_filecheck );
        else throw runtime_error( "Couldn't find resource '"+ result +"', does it exist in your resources directory? Is the spelling correct?" );

      return result;
    }

    string getResourcePath()
    {
      return getApplicationDirectory();
    }

    string buildUserDataPath( const string& filename )
    {
      string  result( filename );
      char         l_path[MAX_PATH];
      char         l_executable[MAX_PATH];
      char         l_filename[_MAX_FNAME];
      FILE        *l_filecheck;

      if (SUCCEEDED( SHGetFolderPathA( NULL, CSIDL_APPDATA, NULL, 0, l_path ) ) && GetModuleFileNameA( 0, l_executable, MAX_PATH ))
      {
        if (_splitpath_s( l_executable, NULL, 0, NULL, 0, l_filename, _MAX_FNAME, NULL, 0 ) == 0 &&
            PathAppendA( l_path, l_filename ) && PathAppendA( l_path, result.c_str() ))
        {
          result = reinterpret_cast<char*>(l_path);
        }
      }

      if (fopen_s(&l_filecheck, result.c_str(), "r") == 0) fclose( l_filecheck );
        else throw runtime_error( "Couldn't find resource '"+ result +"', does it exist in your user data directory? Is the spelling correct?" );

      return result;
    }

  }
}
