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
    long currentTimeMicroSeconds()
    {
      FILETIME      filetime;
      LARGE_INTEGER timevalue;

      GetSystemTimeAsFileTime( &filetime );
      timevalue.LowPart  = filetime.dwLowDateTime;
      timevalue.HighPart = filetime.dwHighDateTime;

      return (long) (timevalue.QuadPart / 10);
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
        else throw std::runtime_error(string("Couldn't find resource '"+ result.string() +"', does it exist in your resources directory? Is the spelling correct?").c_str());

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
        else throw std::runtime_error(string("Couldn't find resource '"+ result.string() +"', does it exist in your user data directory? Is the spelling correct?").c_str());

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

    string getThreadName() {
      return "";
    }

    /**
     see: http://www.codeproject.com/KB/clipboard/clipboard_faq.aspx
     */
    bool setClipboardString(const string& str) {
      bool result = false;
      if(OpenClipboard(NULL)) {
	      HGLOBAL clipbuffer;
	      char * buffer;
	      EmptyClipboard();
	      clipbuffer = GlobalAlloc(GMEM_DDESHARE, str.size()+1);
	      buffer = (char*)GlobalLock(clipbuffer);
        strcpy(buffer, str.c_str());
	      GlobalUnlock(clipbuffer);
	      result = SetClipboardData(CF_TEXT,clipbuffer) != NULL;
	      CloseClipboard();
      }
      return result;
    }
    
    /**
     see: http://www.codeproject.com/KB/clipboard/clipboard_faq.aspx
     */
    string getClipboardString() {
      string result;
      if (OpenClipboard(NULL)) {
	      HANDLE hData = GetClipboardData(CF_TEXT);
	      char * buffer = (char*)GlobalLock(hData);
	      result = buffer;
	      GlobalUnlock(hData);
	      CloseClipboard();
      }
      return result;
    }
    
  }
}
