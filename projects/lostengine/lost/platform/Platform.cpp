#include "lost/platform/Platform.h"

namespace lost
{
  namespace platform
  {

    // returns current time in seconds
    double currentTimeSeconds()
    {
      return (currentTimeMicroSeconds() / 1000000.0);
    }

    // returns current time in milliseconds
    double currentTimeMilliSeconds()
    {
      return (currentTimeMicroSeconds() / 1000.0);
    }

    PlatformType getPlatform()
    {
      #if defined WIN32
        return platform_windows;
      #elif defined __APPLE__
        #if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
          return platform_iphone;
        #else
          return platform_mac;
        #endif
      #elif defined linux
        return platform_linux;
      #endif
    }

    bool isWindows()
    {
      return (getPlatform() == platform_windows);
    }

    bool isMac()
    {
      return (getPlatform() == platform_mac);
    }

    bool isLinux()
    {
      return (getPlatform() == platform_linux);
    }

    bool isIPhone()
    {
      return (getPlatform() == platform_iphone);
    }

    Endianness getEndianness()
    {
      short int word = 0x0001;
      char     *byte = (char *) &word;

      return (byte[0]) ? endLittleEndian : endBigEndian;
    }

    bool isBigEndian()
    {
      return (getEndianness() == endBigEndian);
    }

    bool isLittleEndian()
    {
      return (getEndianness() == endLittleEndian);
    }

  }
}
