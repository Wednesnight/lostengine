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
// IOS

#include <stdexcept>
#include <sys/time.h>
#import <UIKit/UIKit.h>
#include "lost/common/Logger.h"

#ifndef EASTL_DEBUG_BREAK
void EASTL_DEBUG_BREAK() {
    raise(SIGTRAP);
}
#endif

namespace lost
{
  namespace platform
  {

    // returns current time as string (e.g. "2007/11/26 23:30:37")
    string currentTimeFormat()
    {
      const size_t bufsize = 30;
      char   timeformat[bufsize];

      struct timeval tv;
      gettimeofday(&tv, NULL);

      strftime( timeformat, bufsize, "%Y/%m/%d %H:%M:%S", localtime(&tv.tv_sec));
      timeformat[19] = '.';
      timeformat[20] = '%';
      timeformat[21] = '0';
      timeformat[22] = '3';
      timeformat[23] = 'd';
      timeformat[24] = 0;
      snprintf(timeformat, bufsize, timeformat, tv.tv_usec/1000);

      return timeformat;
    }

    // returns current time in microseconds
    long currentTimeMicroSeconds()
    {
      struct timeval tv;

      gettimeofday(&tv, NULL);
      return tv.tv_sec*1000000 + tv.tv_usec;
    }

    // TODO: getApplicationDirectory() not implemented
    lost::fs::Path getApplicationDirectory()
    {
      lost::fs::Path result;
      return result;
    }

    lost::fs::Path getResourcePath()
    {
      CFBundleRef mainBundle = CFBundleGetMainBundle();
      CFURLRef resourceDirURL = CFBundleCopyResourcesDirectoryURL(mainBundle);

      if(!resourceDirURL)
      {
        throw std::runtime_error( "Couldn't find resource directory, does it exist?" );
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
        throw std::runtime_error("couldn't convert CFURL to path for resource dir");
      }
      
      string result(reinterpret_cast<char*>(buffer));
      CFRelease(resourceDirURL);
            
      return lost::fs::Path(result);
    }
    
    lost::fs::Path getUserDataPath()
    {
      lost::fs::Path result;
      
      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
      NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
      if(paths.count > 0)
      {
        NSString* path = [paths objectAtIndex:0];
        const char* cpath = [path cStringUsingEncoding:NSUTF8StringEncoding];
        if(cpath)
        {
          string stringPath(cpath);
          result = stringPath;
        }
        else 
        {
          WOUT("conversion to c-string returned NULL");
        }
      }
      else
      {
        WOUT("couldn't find user data path");
      }
      [pool release];
      return result;
    }

    void setThreadName(const string& name)
    {
      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
      NSString* objcName = [NSString stringWithCString:name.c_str() encoding:NSUTF8StringEncoding];
      [[NSThread currentThread] setName:objcName]; // For Cocoa  
      pthread_setname_np(name.c_str());
      [pool release];
    }

    string getThreadName() {
      return "";
    }

    string getClipboardString()
    {
      string str;
      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
      UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
      NSString* s = [pasteboard string];
      if (s != nil) {
        str = string([s cStringUsingEncoding: NSUTF8StringEncoding]);
      }
      [pool drain];
      return str;
    }
    
    bool setClipboardString(const string& str)
    {
      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
      UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
      NSString* s = [[NSString alloc] initWithUTF8String: str.c_str()];
      [pasteboard setString: s];
      [s release];
      [pool drain];
      return true;
    }

  }
}
