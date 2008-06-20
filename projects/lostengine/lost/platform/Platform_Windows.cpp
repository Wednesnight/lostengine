// platform specific code, forwarded in Platform.h
// WINDOWS

#include <string>
#include <stdexcept>
#include <shlobj.h>
#include <shlwapi.h>
#include <iostream>

using namespace std;

namespace lost
{
  namespace platform
  {

//*** HELPER FUNCTIONS

    void splitApplicationPath( std::string* drive,    std::string* directory,
                               std::string* filename, std::string* extension )
    {
      char   l_executable[MAX_PATH];
      char   l_drive[_MAX_DRIVE];
      char   l_directory[_MAX_DIR];
      char   l_filename[_MAX_FNAME];
      char   l_extension[_MAX_EXT];

      if (GetModuleFileName( 0, l_executable, MAX_PATH ))
      {
        _splitpath( l_executable, l_drive, l_directory, l_filename, l_extension );
        if (drive)     *drive     = reinterpret_cast<char*>(l_drive);
        if (directory) *directory = reinterpret_cast<char*>(l_directory);
        if (filename)  *filename  = reinterpret_cast<char*>(l_filename);
        if (extension) *extension = reinterpret_cast<char*>(l_extension);
      }
    }

//***

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
      FILETIME      filetime;
      LARGE_INTEGER timevalue;

      GetSystemTimeAsFileTime( &filetime );
      timevalue.LowPart  = filetime.dwLowDateTime;
      timevalue.HighPart = filetime.dwHighDateTime;

      return timevalue.QuadPart / 10;
    }

    std::string getApplicationDirectory()
    {
      string result;
      string l_drive;
      string l_directory;
      char   l_path[MAX_PATH];

      splitApplicationPath( &l_drive, &l_directory, NULL, NULL );
      _makepath( l_path, l_drive.c_str(), l_directory.c_str(), NULL, NULL );
      PathAddBackslash( l_path );
      result = reinterpret_cast<char*>(l_path);

      return result;
    }

    std::string getApplicationFilename( bool excludeExtension )
    {
      string l_filename;
      string l_extension;

      splitApplicationPath( NULL, NULL, &l_filename, &l_extension );
      return (!excludeExtension && !l_extension.empty()) ? l_filename.append( l_extension ) : l_filename;
    }

    std::string buildResourcePath( const std::string& filename )
    {
      std::string result( getApplicationDirectory().append( filename ) );

      FILE *l_filecheck;
      l_filecheck = fopen( result.c_str(), "r" );
      if (l_filecheck) fclose( l_filecheck );
        else throw runtime_error( "Couldn't find resource '"+ result +"', does it exist in your resources directory? Is the spelling correct?" );

      return result;
    }

    std::string getResourcePath()
    {
      return getApplicationDirectory();
    }

    std::string buildUserDataPath( const std::string& filename )
    {
      std::string  result( filename );
      char         l_path[MAX_PATH];
      char         l_executable[MAX_PATH];
      char         l_filename[_MAX_FNAME];
      FILE        *l_filecheck;

      if (SUCCEEDED( SHGetFolderPath( NULL, CSIDL_APPDATA, NULL, 0, l_path ) ) && GetModuleFileName( 0, l_executable, MAX_PATH ))
      {
        _splitpath( l_executable, NULL, NULL, l_filename, NULL );
        if (PathAppend( l_path, l_filename ) && PathAppend( l_path, result.c_str() ))
        {
          result = reinterpret_cast<char*>(l_path);
        }
      }

      l_filecheck = fopen( result.c_str(), "r" );
      if (l_filecheck) fclose( l_filecheck );
        else throw runtime_error( "Couldn't find resource '"+ result +"', does it exist in your user data directory? Is the spelling correct?" );

      return result;
    }

  }
}
