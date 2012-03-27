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

#include "lost/platform/Platform.h"

namespace lost
{
  namespace platform
  {

    // returns current time in seconds
    double currentTimeSeconds()
    {
      return ((double) currentTimeMicroSeconds() / 1000000.0);
    }

    // returns current time in milliseconds
    long currentTimeMilliSeconds()
    {
      return (long) (currentTimeMicroSeconds() / 1000);
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
      #elif defined ANDROID
        return platform_android;
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
