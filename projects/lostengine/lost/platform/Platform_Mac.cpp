// platform specific code, forwarded in Platform.h
// MAC

#include <CoreFoundation/CoreFoundation.h>
#include <string>
#include <stdexcept>
#include <sys/time.h>

using namespace std;

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

    // TODO: getApplicationDirectory() not implemented
    std::string getApplicationDirectory()
    {
      std::string result;
      return result;
    }

    // TODO: getApplicationFilename() not implemented
    std::string getApplicationFilename( bool excludeExtension )
    {
      std::string result;
      return result;
    }

    std::string buildResourcePath( const std::string& filename )
    {
      CFBundleRef mainBundle = CFBundleGetMainBundle();
      CFURLRef    tilesFileURL;

      CFStringRef cffilename  = CFStringCreateWithCString(NULL, filename.c_str(), kCFStringEncodingISOLatin1);
      CFStringRef cfextension = CFStringCreateWithCString(NULL, "", kCFStringEncodingISOLatin1);

      tilesFileURL = CFBundleCopyResourceURL( mainBundle,
                                              cffilename,
                                              cfextension,
                                              NULL );

      CFRelease(cffilename);
      CFRelease(cfextension);

      if(!tilesFileURL)
      {
        throw runtime_error( "Couldn't find resource '"+ filename +"', does it exist in your resources directory? Is the spelling correct?" );
      }

      const unsigned long bufsize = 4096;
      UInt8 buffer[bufsize];
      buffer[0] = 0;
      Boolean convresult = CFURLGetFileSystemRepresentation( tilesFileURL,
                                                             true,
                                                             buffer,
                                                             bufsize );

      if(!convresult)
      {
        throw runtime_error("couldn't convert CFURL to path for resource: "+filename);
      }

      string tilesfilePath(reinterpret_cast<char*>(buffer));
      CFRelease(tilesFileURL);

      return tilesfilePath;
    }

    std::string getResourcePath()
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
      
      return result;
    }

    // TODO: buildUserDataPath() not implemented
    std::string buildUserDataPath( const std::string& filename )
    {
      std::string home = std::getenv("HOME");
      std::string result( home +"/"+ filename );
      return result;
    }

  }
}
