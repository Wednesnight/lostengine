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

- (void)applicationDidFinishLaunching: (NSNotification *)notification
{
  DOUT("applicationDidFinishLaunching");
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
      NSAutoreleasePool*   pool;
      NSApplication*       application;
      ApplicationDelegate* delegate;
    };

    void Application::initialize()
    {
      DOUT("Application::initialize()");

      // initialize hiddenMembers
      hiddenMembers = new ApplicationHiddenMembers;

      // init pool
      hiddenMembers->pool = [[NSAutoreleasePool alloc] init];

      // init app
      hiddenMembers->application = [NSApplication sharedApplication];

      // init delegate
      hiddenMembers->delegate = [ApplicationDelegate alloc];
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
      [NSApp release];
      [hiddenMembers->delegate release];
      [hiddenMembers->pool release];
      delete hiddenMembers;
    }

    void Application::doRun()
    {
      [NSApp run];
    }

    void Application::doQuit()
    {
      [hiddenMembers->delegate performSelectorOnMainThread: @selector(terminate) withObject: nil waitUntilDone: NO];
    }
    
  }
}
