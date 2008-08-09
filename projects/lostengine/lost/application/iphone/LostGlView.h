#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

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
	
	// An animation timer that, when animation is started, will periodically call -drawView at the given rate.
//	NSTimer *animationTimer;
//	NSTimeInterval animationInterval;
	
	// Delegate to do our drawing, called by -drawView, which can be called manually or via the animation timer.
	id<LostGlViewDelegate> delegate;
	
	// Flag to denote that the -setupView method of a delegate has been called.
	// Resets to NO whenever the delegate changes.
	BOOL delegateSetup;
}

@property(nonatomic, assign) id<LostGlViewDelegate> delegate;

//-(void)startAnimation;
//-(void)stopAnimation;
-(void)drawView;

- (id)initGLES;
- (BOOL)createFramebuffer;
- (void)destroyFramebuffer;
- (void)swapBuffers;
//@property NSTimeInterval animationInterval;

@end

@protocol LostGlViewDelegate<NSObject>

@required

// Draw with OpenGL ES
-(void)drawView:(LostGlView*)view;

@optional

// Called whenever you need to do some initialization before rendering.
-(void)setupView:(LostGlView*)view;

@end