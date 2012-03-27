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

#include "lost/resource/DefaultLoader.h"
#include "lost/application/Application.h"
#include <iostream>
#include <Foundation/NSBundle.h>
#include <Foundation/NSAutoreleasePool.h>
#include <AppKit/NSNibLoading.h>
#include <AppKit/NSMenu.h>
#include <AppKit/NSEvent.h>
#include <AppKit/NSCursor.h>
#import "lost/application/mac/ApplicationDelegate.h"
#include "lost/common/Logger.h"


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
      // Cocoa needs a NSAutoreleasePool in place
      NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

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
        NSMenu *main_menu = [[NSMenu allocWithZone: [NSMenu menuZone]] initWithTitle: @"temp"];
        [NSApp setMainMenu: main_menu];
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

        [menu_item release];
        [temp_item release];
        [menu release];
        [main_menu release];
      }
      /*************************************************************/

      [pool release];
    }

    void Application::finalize()
    {
      DOUT("Application::finalize()");
      [hiddenMembers->delegate release];
      delete hiddenMembers;
    }

    void Application::run(int argc, char *argv[])
    {
      [NSApp run];
    }

    void Application::shutdown()
    {
      [hiddenMembers->delegate performSelectorOnMainThread: @selector(terminate) withObject: nil waitUntilDone: NO];
    }
    
    void Application::showMouse(bool visible)
    {
      if(visible) [NSCursor unhide];
        else [NSCursor hide];
    }

    void Application::processEvents(const ProcessEventPtr& event)
    {
      [hiddenMembers->delegate performSelectorOnMainThread: @selector(processApplicationEvents) withObject: nil waitUntilDone: NO];
    }

  }
}
