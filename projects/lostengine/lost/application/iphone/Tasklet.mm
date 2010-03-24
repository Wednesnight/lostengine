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
    };

    void Tasklet::start()
    {
      DOUT("");
      init();

      if(startup())
      {
        isAlive = true;

        // FIXME: fake resize event from here, window size won't probably ever change so we're fine
        lost::shared_ptr<lost::application::ResizeEvent> resizeEvent(new lost::application::ResizeEvent(320, 480));
        eventDispatcher->dispatchEvent(resizeEvent);
      
        DOUT("startup ok, starting DisplayLink");
        hiddenMembers.reset(new TaskletHiddenMembers);
        hiddenMembers->displayLink = [[TaskletDisplayLink alloc] initWithTasklet:this];
        [hiddenMembers->displayLink start];
      }
      else
      {
        dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));
      }
    }

    void Tasklet::run()
    {
        if(update())
        {
          window->context->makeCurrent();
          window->context->defaultFramebuffer();
        
          render();
          processEvents();
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
