#ifndef LOST_PLATFORM_PLATFORM_H
#define LOST_PLATFORM_PLATFORM_H


// TargetConditionals defines the IPHONE Macros we require
// this header is only present if we build on an apple platform
#if defined __APPLE__
  #include <TargetConditionals.h>
#endif

#ifndef TARGET_OS_IPHONE
  #define TARGET_OS_IPHONE 0
#endif
#ifndef TARGET_IPHONE_SIMULATOR
  #define TARGET_IPHONE_SIMULATOR 0
#endif

#include <string>

#if defined WIN32
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #include <windows.h>

  #ifndef __FUNCTION__
    #ifdef __func__
      #define __FUNCTION__ __func__
    #else
      #define __FUNCTION__ "unknown"
    #endif
  #endif

  #ifndef __PRETTY_FUNCTION__
    #ifdef __FUNCDNAME__
      #define __PRETTY_FUNCTION__ __FUNCDNAME__
    #else
      #define __PRETTY_FUNCTION__ "unknown"
    #endif
  #endif
#endif

#include "boost/filesystem.hpp"

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
      platform_iphone  = 3,
      platform_android = 4,
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

    // returns the application resource directory
    // Mac OS X: when building a Cocoa app, the resource directory is located inside
    //           the application bundle
    // Windows : the directory the main executable remains in
    // Linux   : not implemented
    boost::filesystem::path getResourcePath();

    // the directory the main executable remains in
    // Mac OS X: not implemented
    // Windows : done
    // Linux   : not implemented
    boost::filesystem::path getApplicationDirectory();

    // returns current time in microseconds
    double currentTimeMicroSeconds();

    // returns current time as formatted string (e.g. "2007/11/26 23:30:37:123")
    std::string currentTimeFormat();

  //***

  }
}

#endif
