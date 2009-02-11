//
//  EAGLView.m
//  Particl0r
//
//  Created by tboll on 10.02.09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#include <iostream>

struct Vec2
{
  float x, y;
  
  Vec2() : x(0.0f), y(0.0f) {}

  inline Vec2 operator +(const Vec2& other)
  {
    Vec2 result;
    result.x = x + other.x;
    result.y = y + other.y;
    return result;
  }
  
  inline void operator +=(const Vec2& other)
  {
    x = x + other.x;
    y = y + other.y;
  }
  
  inline Vec2 operator *(const double other)
  {
    Vec2 result;
    result.x = x*other;
    result.y = y*other;
    return result;
  }
};

struct Particle
{
  Vec2 position;
  Vec2 speed;
  Vec2 acceleration;

  double lifeCycleMilliSeconds;
  double ageMilliSeconds;

  Particle() : lifeCycleMilliSeconds(0.0), ageMilliSeconds(0.0) {}

  void update(const double elapsedTimeSeconds)
  {
    ageMilliSeconds += elapsedTimeSeconds*1000;
    if (ageMilliSeconds < lifeCycleMilliSeconds)
    {
      position += speed * elapsedTimeSeconds + acceleration * 0.5 * (elapsedTimeSeconds*elapsedTimeSeconds);
      speed += acceleration * elapsedTimeSeconds;
    }
  }
};

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

#import "EAGLView.h"

#define USE_DEPTH_BUFFER 0

// A class extension to declare private methods
@interface EAGLView ()

@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) NSTimer *animationTimer;

- (BOOL) createFramebuffer;
- (void) destroyFramebuffer;

@end


@implementation EAGLView

@synthesize context;
@synthesize animationTimer;
@synthesize animationInterval;


// You must implement this method
+ (Class)layerClass {
    return [CAEAGLLayer class];
}


//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder {
    
    if ((self = [super initWithCoder:coder])) {
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
        if (!context || ![EAGLContext setCurrentContext:context]) {
            [self release];
            return nil;
        }
        
        animationInterval = 1.0 / 60.0;
    }
    return self;
}

#define NUM_PARTICLES 100
- (void)drawView
{
  [EAGLContext setCurrentContext:context];
  
  glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
  glViewport(0, 0, backingWidth, backingHeight);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrthof(-backingWidth/2, backingWidth/2, -backingHeight/2, backingHeight/2, -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);
  
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  static Particle particles[NUM_PARTICLES];
  GLfloat vertices[NUM_PARTICLES*2];
  GLubyte colors[NUM_PARTICLES*4];
  for (unsigned int idx = 0; idx < NUM_PARTICLES; idx++)
  {
    Particle& p = particles[idx];

    float diff = ((float)(idx+1)/NUM_PARTICLES);
    diff = (diff >= 0.5) ? 0.5-(diff-0.5) : diff;

    if (p.ageMilliSeconds >= p.lifeCycleMilliSeconds)
    {
      p.position.x = 0;
      p.position.y = -backingHeight/2;
      p.speed.x = -25.0 + 50.0*((float)(idx+1)/NUM_PARTICLES);
      p.speed.y = 100;
      p.acceleration.x = 0;
      p.acceleration.y = -17.5 + 10.0*diff;
      p.lifeCycleMilliSeconds = 10000;
      p.ageMilliSeconds = 0;
    }
    p.update(1.0 / 60.0);
//    std::cout << p.position.x << ", " << p.position.y << std::endl;

    vertices[idx*2]   = p.position.x;
    vertices[idx*2+1] = p.position.y;

    float factor = (p.ageMilliSeconds / p.lifeCycleMilliSeconds)*(1.0-diff);
    colors[idx*4]   = factor*128;
    colors[idx*4+1] = fmaxf((1-factor)*255, 128);
    colors[idx*4+2] = factor*128;
    colors[idx*4+3] = 255;
  }

  glPointSize(2.0f);

  glVertexPointer(2, GL_FLOAT, 0, vertices);
  glEnableClientState(GL_VERTEX_ARRAY);
  glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
  glEnableClientState(GL_COLOR_ARRAY);

  glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
  
  glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
  [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}


- (void)layoutSubviews {
    [EAGLContext setCurrentContext:context];
    [self destroyFramebuffer];
    [self createFramebuffer];
    [self drawView];
}


- (BOOL)createFramebuffer {
    
    glGenFramebuffersOES(1, &viewFramebuffer);
    glGenRenderbuffersOES(1, &viewRenderbuffer);
    
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
    
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
    
    if (USE_DEPTH_BUFFER) {
        glGenRenderbuffersOES(1, &depthRenderbuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
        glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
    }
    
    if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
        return NO;
    }
    
    return YES;
}


- (void)destroyFramebuffer {
    
    glDeleteFramebuffersOES(1, &viewFramebuffer);
    viewFramebuffer = 0;
    glDeleteRenderbuffersOES(1, &viewRenderbuffer);
    viewRenderbuffer = 0;
    
    if(depthRenderbuffer) {
        glDeleteRenderbuffersOES(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
}


- (void)startAnimation {
    self.animationTimer = [NSTimer scheduledTimerWithTimeInterval:animationInterval target:self selector:@selector(drawView) userInfo:nil repeats:YES];
}


- (void)stopAnimation {
    self.animationTimer = nil;
}


- (void)setAnimationTimer:(NSTimer *)newTimer {
    [animationTimer invalidate];
    animationTimer = newTimer;
}


- (void)setAnimationInterval:(NSTimeInterval)interval {
    
    animationInterval = interval;
    if (animationTimer) {
        [self stopAnimation];
        [self startAnimation];
    }
}


- (void)dealloc {
    
    [self stopAnimation];
    
    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }
    
    [context release];  
    [super dealloc];
}

@end
