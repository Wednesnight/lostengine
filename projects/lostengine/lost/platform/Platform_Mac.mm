// platform specific code, forwarded in Platform.h
// MAC

#include <CoreFoundation/CoreFoundation.h>
#include <stdexcept>
#include <sys/time.h>
#include "boost/filesystem.hpp"
#import <Foundation/Foundation.h>
#include "lost/common/Logger.h"
#import <AppKit/NSPasteboard.h>

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
    double currentTimeMicroSeconds()
    {
      struct timeval tv;

      gettimeofday(&tv, NULL);
      return ((double)tv.tv_sec)*1000000.0 + (double)tv.tv_usec;
    }

    // TODO: getApplicationDirectory() not implemented
    boost::filesystem::path getApplicationDirectory()
    {
      boost::filesystem::path result;
      return result;
    }

    boost::filesystem::path getResourcePath()
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
            
      return boost::filesystem::path(result);
    }
    
    boost::filesystem::path getUserDataPath()
    {
      boost::filesystem::path result;
      
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

    string getClipboardString()
    {
      string str;
      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
      NSPasteboard* pboard = [NSPasteboard pasteboardWithName: NSGeneralPboard];
      if ([[pboard types] containsObject: NSStringPboardType]) {
        NSString* s = [pboard stringForType: NSStringPboardType];
        if (s != nil) {
          str = string([s cStringUsingEncoding: NSUTF8StringEncoding]);
        }
      }
      [pool drain];
      return str;
    }
    
    bool setClipboardString(const string& str)
    {
      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
      NSPasteboard* pboard = [NSPasteboard pasteboardWithName: NSGeneralPboard];
      [pboard declareTypes: [NSArray arrayWithObject: NSStringPboardType] owner: nil];
      NSString* s = [[NSString alloc] initWithUTF8String: str.c_str()];
      bool result = [pboard setString: s forType: NSStringPboardType];
      [s release];
      [pool drain];
      return result;
    }

  }
}
