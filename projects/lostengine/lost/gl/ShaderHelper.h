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
