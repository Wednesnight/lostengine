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
