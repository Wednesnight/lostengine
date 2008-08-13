#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#include "lost/application/TouchEvent.h"
#include <list>
@protocol LostGlViewDelegate;

@interface LostGlView : UIView
{
	@private
	// The pixel dimensions of the backbuffer
	GLint backingWidth;
	GLint backingHeight;
	
	EAGLContext *context;
	
	// OpenGL names for the renderbuffer and framebuffers used to render to this view
	GLuint viewRenderbuffer, viewFramebuffer;
	
	// OpenGL name for the depth buffer that is attached to viewFramebuffer, if it exists (0 if it does not exist)
	GLuint depthRenderbuffer;
		
	// Delegate to do our drawing, called by -drawView, which can be called manually or via the animation timer.
	id<LostGlViewDelegate> delegate;
	  
  boost::shared_ptr<lost::application::TouchEvent>* touchEvent; 
  std::list<boost::shared_ptr<lost::application::TouchEvent::Touch> >* touches;
  NSUInteger maxNumTouches;
}

@property(nonatomic, assign) id<LostGlViewDelegate> delegate;

-(void)drawView;
- (id)initGLES;
- (BOOL)createFramebuffer;
- (void)destroyFramebuffer;
- (void)swapBuffers;

@end

// ------------------------------------------------------
@protocol LostGlViewDelegate<NSObject>

@required

// Draw with OpenGL ES
-(void)drawView:(LostGlView*)view;

@end