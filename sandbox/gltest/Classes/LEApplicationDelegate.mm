#import "LEApplicationDelegate.h"
#import "LEWindow.h"
#import <QuartzCore/CADisplayLink.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import "LEGLView.h"

@implementation LEApplicationDelegate

-(void)render:(id)sender
{
  [EAGLContext setCurrentContext:_window.leglView.context];
  glBindFramebuffer(GL_FRAMEBUFFER, _window.leglView.defaultFramebuffer);GLDEBUG;
  glViewport(0, 0, _window.leglView.backingWidth, _window.leglView.backingHeight);GLDEBUG;

  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);GLDEBUG;
  glClear(GL_COLOR_BUFFER_BIT);GLDEBUG;
  [_window.leglView.context presentRenderbuffer:GL_RENDERBUFFER];  
}

-(void)startDisplayLink
{
  _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
  [_displayLink setFrameInterval:1];
  [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];  
}

-(void)stopDisplayLink
{
  [_displayLink invalidate];
  _displayLink = nil;
}

- (void) applicationDidFinishLaunching:(UIApplication *)application
{
  NSLog(@"%s", __FUNCTION__);
  _window = [[LEWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
  [_window makeKeyAndVisible];
  [self startDisplayLink];
}

- (void) applicationWillResignActive:(UIApplication *)application
{
  NSLog(@"%s", __FUNCTION__);
}

- (void) applicationDidBecomeActive:(UIApplication *)application
{
  NSLog(@"%s", __FUNCTION__);
}

- (void)applicationWillTerminate:(UIApplication *)application
{
  NSLog(@"%s", __FUNCTION__);
  [self stopDisplayLink];
}

- (void) dealloc
{
  NSLog(@"%s", __FUNCTION__);
  [_window release];
	[super dealloc];
}

@end