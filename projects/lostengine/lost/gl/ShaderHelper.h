#ifndef LOST_GL_SHADERHELPER_H
#define LOST_GL_SHADERHELPER_H

#include "lost/gl/ShaderProgram.h"
#include "lost/gl/FragmentShader.h"
#include "lost/gl/VertexShader.h"
#include "lost/common/FileHelpers.h"
#include <stdexcept>
#include <string>
#include <boost/shared_ptr.hpp>

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
boost::shared_ptr<lost::gl::ShaderProgram> loadShader(const std::string& inName)
{
  boost::shared_ptr<lost::gl::FragmentShader> fragmentShader(new lost::gl::FragmentShader());
  boost::shared_ptr<lost::gl::VertexShader>   vertexShader(new lost::gl::VertexShader());
  boost::shared_ptr<lost::gl::ShaderProgram>  shaderProgram(new lost::gl::ShaderProgram());

  vertexShader->source(lost::common::loadFile(inName+".vs"));
  vertexShader->compile();
  if(!vertexShader->compiled())
    throw std::runtime_error(vertexShader->log());

  fragmentShader->source(lost::common::loadFile(inName+".fs"));
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
