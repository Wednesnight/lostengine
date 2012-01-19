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

#include "lost/application/Tasklet.h"
#include "lost/application/TaskletEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/event/EventDispatcher.h"
#include "lost/common/Logger.h"
#import <QuartzCore/CADisplayLink.h>
#import <Foundation/NSRunLoop.h>
#include "lost/application/Window.h"
#include "lost/gl/Context.h"

@interface TaskletDisplayLink : NSObject
{
  CADisplayLink* displayLink;
  lost::application::Tasklet* tasklet;
}

-(id)initWithTasklet:(lost::application::Tasklet*)t;
-(void)update:(id)sender;
-(void)start;
-(void)stop;

@end

@implementation TaskletDisplayLink

-(id)initWithTasklet:(lost::application::Tasklet*)t
{
  if(self = [super init])
  {
    tasklet = t;
  }
  return self;
}

-(void)update:(id)sender
{
  tasklet->run();
}

-(void)start
{
  displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update:)];
  [displayLink setFrameInterval:1];
  [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];  
}

-(void)stop
{
  if(displayLink)
  {
    [displayLink invalidate];
    displayLink = nil;
  }
}


@end

namespace lost
{
  namespace application
  {

    struct Tasklet::TaskletHiddenMembers
    {
      TaskletDisplayLink* displayLink;
      double offset;
    };

    void Tasklet::start()
    {
      DOUT("");
      init();
      startup();
      if(running)
      {
        isAlive = true;
        // FIXME: fake resize event from here, window size won't probably ever change so we're fine
        lost::shared_ptr<lost::application::ResizeEvent> resizeEvent(new lost::application::ResizeEvent(320, 480));
        eventDispatcher->dispatchEvent(resizeEvent);
      
        DOUT("startup ok, starting DisplayLink");
        hiddenMembers.reset(new TaskletHiddenMembers);
        hiddenMembers->displayLink = [[TaskletDisplayLink alloc] initWithTasklet:this];
        [hiddenMembers->displayLink start];
        hiddenMembers->offset = clock.getTime();
      }
      else
      {
        dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));
      }
    }

    void Tasklet::run()
    {
        processEvents();
        double framerate = clock.getElapsedAndUpdateOffset(hiddenMembers->offset);
        update(framerate);
        if(running)
        {
          window->context->makeCurrent();
          window->context->defaultFramebuffer();
        
          render();
        }
        else
        {
          isAlive = false;
          shutdown();
          dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));
        }
    }

    void Tasklet::stop()
    {
      DOUT("");
      if(isAlive==true)
      {
        if(hiddenMembers && hiddenMembers->displayLink)
        {
          [hiddenMembers->displayLink stop];
          [hiddenMembers->displayLink release];
          hiddenMembers.reset();
        }
        isAlive = false;
        shutdown();
        dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));
      }
      else
      {
        cleanup();
      }
    }

  }
}
