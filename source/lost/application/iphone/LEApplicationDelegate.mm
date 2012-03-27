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
