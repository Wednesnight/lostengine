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
  lost::shared_ptr<lost::gl::FragmentShader> fragmentShader(new lost::gl::FragmentShader());
  lost::shared_ptr<lost::gl::VertexShader>   vertexShader(new lost::gl::VertexShader());
  lost::shared_ptr<lost::gl::ShaderProgram>  shaderProgram(new lost::gl::ShaderProgram());

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