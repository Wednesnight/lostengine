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

/** This is the main header for the lgl GL abstraction layer.
 * lgl attempts to provide a common interface to gl functions that are different betwee
 * OpenGL and OpenGL ES.
 * Additionally, enums and types are rovided where required.
 * lglu.h uses these functions to reimplement glu functions so they can be used on both OpenGL and
 * OpenGL ES.
 */

#ifndef LOST_LGL_LGL_H
#define LOST_LGL_LGL_H

#if defined __APPLE__
  #include <TargetConditionals.h>
#endif

#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE || defined ANDROID
  #include "lost/lgl/lgl_gles.h"
#else
  #include "lost/lgl/lgl_gl.h"
#endif

#endif
