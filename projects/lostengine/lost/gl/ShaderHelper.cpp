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

#include "lost/gl/ShaderHelper.h"
#include "lost/gl/FragmentShader.h"
#include "lost/gl/VertexShader.h"
//#include <stdexcept>
#include "lost/common/Data.h"
#include "lost/common/Logger.h"
//#include <iomanip>
//#include <sstream>
#include "lost/lua/lostlua.h"
#include "lost/lua/State.h"

namespace lost
{
namespace gl
{

string processDependencies(const resource::LoaderPtr& loader, const string& source, lua_State* state)
{
  slub::lua_function<string, resource::LoaderPtr, string> func =
    slub::globals(state)["lost"]["gl"]["preprocessShader"];
  return func(loader, source);
}

// logs shader source with line numbers
void broken(const string& source, lua_State* state)
{
  lua::State* lstate = lua::State::stateFromState(state);
  slub::reference ppfunc = lstate->globals["lost"]["gl"]["logBrokenShader"];
  slub::lua_function<void, string> func(ppfunc);
  func(source);  
}

ShaderProgramPtr buildShader(const resource::LoaderPtr& loader, const string& inName, const string& vssource, const string& fssource, lua_State* state)
{
//  DOUT("building shader: '"<<inName<<"'");
  lost::gl::ShaderProgramPtr  shaderProgram(new lost::gl::ShaderProgram());
  lost::gl::ShaderPtr         vertexShader(new lost::gl::VertexShader());
  lost::gl::ShaderPtr         fragmentShader(new lost::gl::FragmentShader());

  string vsname = inName+".vs";
  string fsname = inName+".fs";

  string source = processDependencies(loader, vssource, state);
  vertexShader->source(source);
  vertexShader->compile();
  if(!vertexShader->compiled())
  {
    broken(source, state);
    THROW_RTE("vertex shader '"<<vsname<<"' compilation failed: "<<vertexShader->log());
  }

  source = processDependencies(loader, fssource, state);
  fragmentShader->source(source);
  fragmentShader->compile();
  if(!fragmentShader->compiled())
  {
    broken(source, state);
    THROW_RTE("fragment shader '"<<fsname<<"' compilation failed: "<<fragmentShader->log());
  }

  shaderProgram->attach(fragmentShader);
  shaderProgram->attach(vertexShader);
  shaderProgram->link();
  if(!shaderProgram->linked())
  {
    THROW_RTE("shader program '"<<inName<<"' link failed: "<<shaderProgram->log());
  }

  shaderProgram->enable();
  shaderProgram->buildUniformMap();
  shaderProgram->buildVertexAttributeMap();
  shaderProgram->disable();
  
  return shaderProgram;
}

ShaderProgramPtr loadShader(const resource::LoaderPtr& loader, const string& inName, lua_State* state)
{
//  DOUT("--- loading shader '"<<inName<<"'");
  string vsname = inName+".vs";
  common::DataPtr vsfile = loader->load(vsname);
  string fsname = inName+".fs";
  common::DataPtr fsfile = loader->load(fsname);
  return buildShader(loader, inName, vsfile->str(), fsfile->str(), state);
}

}
}
