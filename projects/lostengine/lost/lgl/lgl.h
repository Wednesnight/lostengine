/** This is the main header for the lgl GL abstraction layer.
 * lgl attempts to provide a common interface to gl functions that are different betwee
 * OpenGL and OpenGL ES.
 * Additionally, enums and types are rovided where required.
 * lglu.h uses these functions to reimplement glu functions so they can be used on both OpenGL and
 * OpenGL ES.
 */

#ifndef LOST_GL_LGL_H
#define LOST_GL_LGL_H

#include "lost/gl/gl.h"

#if defined(TARGET_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
# include "lost/lgl/lgl_gles.h"
#else
# include "lost/lgl/lgl_gl.h"
#endif

#endif
