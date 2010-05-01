#include "lost/gl/ShaderHelper.h"
#include "lost/gl/FragmentShader.h"
#include "lost/gl/VertexShader.h"
#include <stdexcept>
#include <boost/filesystem.hpp>

namespace lost
{
namespace gl
{

ShaderProgramPtr loadShader(lost::shared_ptr<lost::resource::Loader> loader, const std::string& inName)
{
  lost::gl::ShaderProgramPtr  shaderProgram(new lost::gl::ShaderProgram());
  lost::gl::ShaderPtr         vertexShader(new lost::gl::VertexShader());
  lost::gl::ShaderPtr         fragmentShader(new lost::gl::FragmentShader());

  common::DataPtr vsfile = loader->load(inName+".vs");
  vertexShader->source(vsfile->str());
  vertexShader->compile();
  if(!vertexShader->compiled())
    throw std::runtime_error(vertexShader->log());

  common::DataPtr fsfile = loader->load(inName+".fs");
  fragmentShader->source(fsfile->str());
  fragmentShader->compile();
  if(!fragmentShader->compiled())
    throw std::runtime_error(fragmentShader->log());

  shaderProgram->attach(fragmentShader);
  shaderProgram->attach(vertexShader);
  shaderProgram->link();
  if(!shaderProgram->linked())
    throw std::runtime_error(shaderProgram->log());

  shaderProgram->enable();
  shaderProgram->buildUniformMap();
  shaderProgram->buildVertexAttributeMap();
  shaderProgram->disable();
  
  return shaderProgram;
}

}
}