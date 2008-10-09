#include <boost/shared_ptr.hpp>
#include "lost/application/Timer.h"

#include <CoreFoundation/CoreFoundation.h>
#include "lost/common/Logger.h"
namespace lost
{
namespace application
{
  
  struct TimerContext
  {
    double lastTime; // set to creation date on startup
    Timer* timer;
    CFRunLoopTimerRef rltimer;    
  };
    
  void updateCallback(CFRunLoopTimerRef timer, void* info)
  {
    TimerContext* tc = static_cast<TimerContext*>(info);
    double now = CFAbsoluteTimeGetCurrent();
    double delta = now - tc->lastTime;
    
    tc->timer->event->timer = tc->timer;
    tc->timer->event->passedSec = delta;
    tc->timer->dispatchEvent(tc->timer->event);
    
    tc->lastTime = now;
  }
  
  Timer::Timer(const std::string& name, double inIntervalSec)
  {
    running = true;
    event.reset(new TimerEvent(TimerEvent::TIMER_FIRED()));
    context.reset(new TimerContext);
    context->timer = this;
    context->lastTime = CFAbsoluteTimeGetCurrent();
    
    CFRunLoopTimerContext rltcontext;
    rltcontext.version = 0;
    rltcontext.info = context.get();
    rltcontext.retain = 0;
    rltcontext.release = 0;
    rltcontext.copyDescription = 0;
    
    context->rltimer = CFRunLoopTimerCreate(kCFAllocatorDefault,
                                            CFAbsoluteTimeGetCurrent(),
                                            inIntervalSec,
                                            0,
                                            0,
                                            updateCallback,
                                            &rltcontext);
        
    CFRunLoopAddTimer(CFRunLoopGetCurrent(), context->rltimer, kCFRunLoopCommonModes);
  }
  
  Timer::~Timer()
  {
    if(CFRunLoopTimerIsValid(context->rltimer))
      CFRunLoopTimerInvalidate(context->rltimer);
    CFRelease(context->rltimer);
  }
}
}