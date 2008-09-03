#ifndef LOST_PLATFORM_PLATFORM_H
#define LOST_PLATFORM_PLATFORM_H

// TargetConditionals defines the IPHONE Macros we require
// this header is only present if we build on an apple platform
#if defined __APPLE__
  #include <TargetConditionals.h>
#endif

#include <string>

#if defined WIN32
  #include <windows.h>
#endif

namespace lost
{
  namespace platform
  {

  //*** cross-platform support

    typedef enum
    {
      platform_windows = 0,
      platform_mac     = 1,
      platform_linux   = 2,
      platform_iphone  = 3
    } PlatformType;

    PlatformType getPlatform();

    bool isWindows();
    bool isMac();
    bool isLinux();
    bool isIPhone();

    typedef enum
    {
      endBigEndian    = 0,
      endLittleEndian = 1
    } Endianness;

    Endianness getEndianness();

    bool isBigEndian();
    bool isLittleEndian();

  //***

  //*** based on platform specific code

    // returns current time in seconds
    double currentTimeSeconds();
    // returns current time in milliseconds
    double currentTimeMilliSeconds();

  //***

  //*** platform specific forwards, implemented in Platform_*.cpp

    // tries to find the resource in the application resource directory
    // throws runtime_exception if resource could not be found
    // Mac OS X: when building a Cocoa app, the resource directory is located inside
    //           the application bundle
    // Windows : the directory the main executable remains in
    // Linux   : not implemented
    std::string buildResourcePath( const std::string& filename );

    // returns the application resource directory
    // Mac OS X: when building a Cocoa app, the resource directory is located inside
    //           the application bundle
    // Windows : the directory the main executable remains in
    // Linux   : not implemented
    std::string getResourcePath();

    // tries to find the resource in the user's data directory
    // throws runtime_exception if file could not be found
    // Mac OS X: not implemented
    // Windows : ~\Documents and Settings\<user>\Application Data\<program name>
    // Linux   : not implemented
    std::string buildUserDataPath( const std::string& filename );

    // the directory the main executable remains in
    // Mac OS X: not implemented
    // Windows : done
    // Linux   : not implemented
    std::string getApplicationDirectory();

    // the main executable filename
    // Mac OS X: not implemented
    // Windows : done
    // Linux   : not implemented
    std::string getApplicationFilename( bool excludeExtension = false );

    // returns current time in microseconds
    double currentTimeMicroSeconds();

    // FIXME: why do we need a string parameter here? it bit me in the ass when I just wanted to log the time from a macro and needed to instaniate a bogus parameter
    // FIXME: name is misleading: we don't return a time format, we return the current time formatted in string form.
    // returns current time as string (e.g. "2007/11/26 23:30:37")
    std::string& currentTimeFormat( std::string& sTime );

  //***

  }
}

#endif
