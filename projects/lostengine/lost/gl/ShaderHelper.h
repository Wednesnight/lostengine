#ifndef LOST_GL_SHADERHELPER_H
#define LOST_GL_SHADERHELPER_H

#include "lost/gl/ShaderProgram.h"
#include "lost/gl/FragmentShader.h"
#include "lost/gl/VertexShader.h"
#include <stdexcept>
#include <string>
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
static inline ShaderProgramPtr loadShader(lost::shared_ptr<lost::resource::Loader> loader, const std::string& inName)
{
  lost::shared_ptr<lost::gl::FragmentShader> fragmentShader(new lost::gl::FragmentShader());
  lost::shared_ptr<lost::gl::VertexShader>   vertexShader(new lost::gl::VertexShader());
  lost::shared_ptr<lost::gl::ShaderProgram>  shaderProgram(new lost::gl::ShaderProgram());

  lost::shared_ptr<resource::File> vsfile = loader->load(inName+".vs");
  vertexShader->source(vsfile->str());
  vertexShader->compile();
  if(!vertexShader->compiled())
    throw std::runtime_error(vertexShader->log());

  lost::shared_ptr<resource::File> fsfile = loader->load(inName+".fs");
  fragmentShader->source(fsfile->str());
  fragmentShader->compile();
  if(!fragmentShader->compiled())
    throw std::runtime_error(fragmentShader->log());

  shaderProgram->attach(fragmentShader);
  shaderProgram->attach(vertexShader);
  shaderProgram->link();
  if(!shaderProgram->linked())
    throw std::runtime_error(shaderProgram->log());

  return shaderProgram;
}

}
}

#endif
