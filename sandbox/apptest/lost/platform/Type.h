#ifndef LOST_PLATFORM_TYPE_H
#define LOST_PLATFORM_TYPE_H

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

namespace lost
{
namespace platform
{
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
}
}

#endif