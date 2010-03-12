#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "lost/gl/gl.h"
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>

// FIXME: put some common defines inside a forward header so we don't have to choose API just yet
typedef int             GLint;
typedef unsigned int    GLuint;

// -----------------

@interface LEGLView : UIView
{
	EAGLContext* _context;
	
	// The pixel dimensions of the CAEAGLLayer
	GLint _backingWidth;
	GLint _backingHeight;
	
	// The OpenGL names for the framebuffer and renderbuffer used to render to this view
	GLuint _defaultFramebuffer;
  GLuint _colorRenderbuffer;
  GLuint _depthRenderbuffer;
}

-(id)initWithFrame:(CGRect)fr;

@property(nonatomic, readonly)	EAGLContext* context;
@property(nonatomic, readonly)	GLint backingWidth;
@property(nonatomic, readonly)	GLint backingHeight;
@property(nonatomic, readonly)	GLuint defaultFramebuffer;
@property(nonatomic, readonly)  GLuint colorRenderbuffer;
@property(nonatomic, readonly)  GLuint depthRenderbuffer;

@end