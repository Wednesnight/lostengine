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

#ifndef LOST_GL_SHADERHELPER_H
#define LOST_GL_SHADERHELPER_H

#include "lost/gl/ShaderProgram.h"
#include "lost/resource/Loader.h"

namespace lost
{
namespace gl
{

/** Helper function for a common/trivial case of loading and compiling a vertex/fragment shader
 * pair, creating a program, compiling and attaching them.
 * Two restrictions:
 *    * vertex and fragment shaders must have the same name with extensions "vs" and
 *      "fs"
 *    * the files must reside in the application resource dir so the standard load functions can find them.
 *
 *  throws if one of the compile stages or the link stage fail.
 */
ShaderProgramPtr loadShader(const resource::LoaderPtr& loader,
                                   const string& inName);

/** Helper function for assembling a shader from vertex/fragment shader data.
 * Resolves imports/includes before compiling/linking the shader.
 */
ShaderProgramPtr buildShader(const resource::LoaderPtr& loader, 
                             const string& inName, 
                             const string& vssource,
                             const string& fssource);

}
}

#endif
