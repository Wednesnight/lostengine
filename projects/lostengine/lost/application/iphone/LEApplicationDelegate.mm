#import "lost/application/iphone/LEApplicationDelegate.h"
//#import "lost/application/iphone/LEWindow.h"
//#import <QuartzCore/CADisplayLink.h>
#import "lost/gl/gl.h"
#import "lost/gl/Utils.h"
//#import <OpenGLES/EAGL.h>
//#import <OpenGLES/EAGLDrawable.h>
//#import "lost/application/iphone/LEGLView.h"
#import "lost/application/Application.h"
#import "lost/application/ApplicationEvent.h"
#import "lost/event/EventDispatcher.h"

extern lost::application::Application* gApplicationPointerForLEApplicationDelegate;

using namespace lost::application;

@implementation LEApplicationDelegate

-(void)render:(id)sender
{
//  [EAGLContext setCurrentContext:_window.leglView.context];
//  glBindFramebuffer(GL_FRAMEBUFFER, _window.leglView.defaultFramebuffer);GLDEBUG;
  glViewport(0, 0, 320, 480);GLDEBUG;

  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);GLDEBUG;
  glClear(GL_COLOR_BUFFER_BIT);GLDEBUG;
//  [_window.leglView.context presentRenderbuffer:GL_RENDERBUFFER];  
}

/*-(void)startDisplayLink
{
  _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
  [_displayLink setFrameInterval:1];
  [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];  
}

-(void)stopDisplayLink
{
  [_displayLink invalidate];
  _displayLink = nil;
}*/

- (void) applicationDidFinishLaunching:(UIApplication *)application
{
  DOUT("");
//  ApplicationEvent* ev1 = ;
  ApplicationEventPtr ev(new ApplicationEvent(ApplicationEvent::RUN()));
  gApplicationPointerForLEApplicationDelegate->eventDispatcher->dispatchEvent(ev);
//  _window = [[LEWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
//  [_window makeKeyAndVisible];
//  [self startDisplayLink];
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
  DOUT("");  
}

- (void) applicationWillResignActive:(UIApplication *)application
{
  DOUT("");
}

- (void) applicationDidBecomeActive:(UIApplication *)application
{
  DOUT("");
}

- (void)applicationWillTerminate:(UIApplication *)application
{
  DOUT("");
  ApplicationEventPtr ev(new ApplicationEvent(ApplicationEvent::QUIT()));
  gApplicationPointerForLEApplicationDelegate->eventDispatcher->dispatchEvent(ev);  
//  [self stopDisplayLink];
  [self render:self];
}

- (void) dealloc
{
  DOUT("");
//  [_window release];
	[super dealloc];
}

@end