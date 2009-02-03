#include "lost/application/Application.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "lost/application/ApplicationEvent.h"
#include "lost/common/Logger.h"

lost::application::Application* currentApplication;

@interface ApplicationDelegate : NSObject <UIApplicationDelegate>
{
  lost::application::Application* parent;
}
@end

@implementation ApplicationDelegate

- (void) dummyThread:(id)arg;
{
}

-(id)init
{
    if(self = [super init])
    {
      NSLog(@"isMultiThreaded %d", [NSThread isMultiThreaded]);
     [NSThread detachNewThreadSelector:@selector(dummyThread:) toTarget:self withObject:nil];
      NSLog(@"isMultiThreaded %d", [NSThread isMultiThreaded]);        
    }
    return self;
}

- (void)applicationDidFinishLaunching:(UIApplication *)application
{
  parent = currentApplication;
  for (std::map<std::string, boost::shared_ptr<lost::application::Window> >::iterator idx = parent->windows.begin(); idx != parent->windows.end(); ++idx)
  {
    (*idx).second->open();
  }
  parent->startRunLoop();
  NSLog(@"isMultiThreaded %d", [NSThread isMultiThreaded]);
}

- (void)applicationWillResignActive:(UIApplication *)application
{
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
}

- (void)applicationWillTerminate: (UIApplication *)application
{
  if (parent) parent->terminate();
}

- (void)terminate
{
  // FIXME: implement ApplicationDelegate::terminate
}

@end

namespace lost
{
  namespace application
  {
    
    struct Application::ApplicationHiddenMembers
    {
      NSAutoreleasePool* pool;
      UIApplication*     application;
    };

    void Application::initialize()
    {
      // initialize hiddenMembers
      hiddenMembers = new ApplicationHiddenMembers;
      
      // init pool
      hiddenMembers->pool = [[NSAutoreleasePool alloc] init];
      
      // init app
      hiddenMembers->application = [UIApplication sharedApplication];
    }

    void Application::finalize()
    {
      [hiddenMembers->pool release];
      delete hiddenMembers;
    }

    void Application::doRun()
    {
      currentApplication = this;
      UIApplicationMain(0, NULL, @"UIApplication", @"ApplicationDelegate");
    }

    void Application::doQuit()
    {
      [hiddenMembers->application performSelectorOnMainThread: @selector(terminate) withObject: nil waitUntilDone: NO];
    }
    
  }
}

// FIXME: remove this old stuff
#import <OpenGLES/EAGL.h>

void* lglGetCurrentContext()
{
  return (void*)[EAGLContext currentContext];
}

bool lglSetCurrentContext(void* context)
{
  return [EAGLContext setCurrentContext:(EAGLContext*)context];
}
