#include "lost/application/Window.h"
#include "lost/common/Logger.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/KeyCode.h"
#include "lost/application/DragNDropEvent.h"
#include "lost/application/WindowEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/application/mac/ApplicationWindow.h"
#include "lost/application/mac/GLView.h"
#import <AppKit/NSOpenGL.h>
#include "lost/gl/Context.h"
#include "lost/application/TaskletConfig.h"

namespace lost
{
  namespace application
  {
        
    struct Window::WindowHiddenMembers
    {
      ApplicationWindow* window;
      GLView*            view;
    };

    void Window::initialize()
    {
      // initialize hiddenMembers
      hiddenMembers = new WindowHiddenMembers;

      // create view
      hiddenMembers->view = [[GLView alloc] initWithFrame: NSMakeRect(config->windowRect.x, config->windowRect.y, config->windowRect.width, config->windowRect.height)];

      // create window
      hiddenMembers->window = [[ApplicationWindow alloc]
                               initWithContentRect: [hiddenMembers->view frame]
                               styleMask: NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask
                               backing: NSBackingStoreBuffered
                               defer: NO];

      // set params
      [hiddenMembers->window setParent: this];
      [hiddenMembers->window setTitle: [[NSString alloc] initWithCString:config->windowTitle.c_str()]];
      [hiddenMembers->window setAcceptsMouseMovedEvents: YES];
      [hiddenMembers->window setContentView: hiddenMembers->view];
      [hiddenMembers->window setDelegate: hiddenMembers->window];
      [hiddenMembers->window setReleasedWhenClosed: NO];

      [[hiddenMembers->view openGLContext] makeCurrentContext];
      context.reset(new gl::Context);
      context->vsync(config->glVsync);
      
      open();
    }

    void Window::finalize()
    {
//      DOUT("Window::finalize()");
      if(hiddenMembers) [hiddenMembers->window setParent: NULL]; else EOUT("hiddenMembers is NULL!");
      
      if(hiddenMembers) context.reset(); else EOUT("hiddenMembers is NULL!"); // prevents crash when it actually is null, dunno why it can become NULL 
      if(hiddenMembers) [hiddenMembers->view release]; else EOUT("hiddenMembers is NULL!");
      if(hiddenMembers) [hiddenMembers->window release]; else EOUT("hiddenMembers is NULL!");
      if(hiddenMembers) delete hiddenMembers; else EOUT("hiddenMembers is NULL!");
    }

    void Window::open()
    {
      [hiddenMembers->window makeKeyAndOrderFront: nil];
    }
    
    void Window::close()
    {
      [hiddenMembers->window performClose: nil];
    }

  }
}
