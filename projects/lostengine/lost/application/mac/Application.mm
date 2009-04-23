#include "lost/resource/DefaultLoader.h"
#include "lost/application/Application.h"

#include <iostream>

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include "lost/application/ApplicationEvent.h"
#include "lost/common/Logger.h"

@interface ApplicationDelegate : NSObject
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
      // make this application multithreaded by spawning a dummy thread that exits immediately
      DOUT("isMultiThreaded: " << (bool)[NSThread isMultiThreaded]);
      [NSThread detachNewThreadSelector:@selector(dummyThread:) toTarget:self withObject:nil];
      DOUT("isMultiThreaded: " << (bool)[NSThread isMultiThreaded]);        
    }
    return self;
}

- (void)applicationDidFinishLaunching: (NSNotification *)notification
{
  DOUT("applicationDidFinishLaunching");
  if (parent)
  {
    parent->dispatchApplicationEvent(lost::application::ApplicationEvent::RUN());
  }
}

- (void)applicationWillTerminate: (NSNotification *)notification
{
  DOUT("applicationWillTerminate");
  if (parent) parent->terminate();
}

- (void)setParent: (lost::application::Application*)newParent
{
  parent = newParent;
}

- (void)terminate
{
  [[NSApplication sharedApplication] terminate: nil];
}

- (void) quitAction: (id)sender
{
  if (parent) parent->quit();
}

@end

/*************************************************************
 * taken from http://www.allegro.cc
 */
/* This prevents warnings that 'NSApplication might not
 * respond to setAppleMenu' on OS X > 10.4
 */
@interface NSApplication(LostApplication)
- (void)setAppleMenu: (NSMenu *)menu;
@end
/*************************************************************/

namespace lost
{
  namespace application
  {
    
    struct Application::ApplicationHiddenMembers
    {
      NSApplication*       application;
      ApplicationDelegate* delegate;
    };

    void Application::initialize()
    {
      // Cocoas needs a NSAutoreleasePool in place
      [[NSAutoreleasePool alloc] init];

      // initialize hiddenMembers
      hiddenMembers = new ApplicationHiddenMembers;

      // init app
      hiddenMembers->application = [NSApplication sharedApplication];

      // init delegate
      hiddenMembers->delegate = [[ApplicationDelegate alloc] init];
      [hiddenMembers->delegate setParent: this];
      
      // set application delegate
      [hiddenMembers->application setDelegate: hiddenMembers->delegate];

      /*************************************************************
       * taken from http://www.allegro.cc
       */
      /* Load the main menu nib if possible */
      if (([NSBundle loadNibNamed: @"MainMenu" owner: NSApp] == NO))
      {
        /* Didn't load the nib; create a default menu programmatically */
        NSMenu *menu;
        NSMenuItem *menu_item, *temp_item;
        NSString* title = nil;
        NSDictionary* app_dictionary = [[NSBundle mainBundle] infoDictionary];
        if (app_dictionary) 
        {
          title = [app_dictionary objectForKey: @"CFBundleName"];
        }
        if (title == nil) 
        {
          title = [[NSProcessInfo processInfo] processName];
        }
        [NSApp setMainMenu: [[NSMenu allocWithZone: [NSMenu menuZone]] initWithTitle: @"temp"]];
        menu = [[NSMenu allocWithZone: [NSMenu menuZone]] initWithTitle: @"temp"];
        temp_item = [[NSMenuItem allocWithZone: [NSMenu menuZone]]
                     initWithTitle: @"temp"
                     action: NULL
                     keyEquivalent: @""];
        [[NSApp mainMenu] addItem: temp_item];
        [[NSApp mainMenu] setSubmenu: menu forItem: temp_item];
        [NSApp setAppleMenu: menu];
        NSString *quit = [@"Quit " stringByAppendingString: title];
        menu_item = [[NSMenuItem allocWithZone: [NSMenu menuZone]]
                     initWithTitle: quit
                     action: @selector(quitAction:)
                     keyEquivalent: @"q"];
        [menu_item setKeyEquivalentModifierMask: NSCommandKeyMask];
        [menu_item setTarget: hiddenMembers->delegate];
        [menu addItem: menu_item];
      }
      /*************************************************************/
    }

    void Application::finalize()
    {
      DOUT("Application::finalize()");
      [hiddenMembers->delegate release];
      delete hiddenMembers;
    }

    void Application::run()
    {
      [NSApp run];
    }

    void Application::shutdown(ApplicationEventPtr& event)
    {
      [hiddenMembers->delegate performSelectorOnMainThread: @selector(terminate) withObject: nil waitUntilDone: NO];
    }
    
    void Application::showMouse(bool visible)
    {
      if(visible) [NSCursor unhide];
        else [NSCursor hide];
    }
  }
}
