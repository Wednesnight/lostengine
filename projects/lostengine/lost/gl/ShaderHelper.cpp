#include "lost/gl/ShaderHelper.h"
#include "lost/gl/FragmentShader.h"
#include "lost/gl/VertexShader.h"
#include <stdexcept>
#include "lost/common/Data.h"
#include "lost/common/Logger.h"
#include <iomanip>
//#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/include/classic_core.hpp>
#include <sstream>
using namespace boost;
using namespace boost::spirit;
using namespace boost::spirit::classic;

namespace lost
{
namespace gl
{

struct loglog
{
  template<typename IteratorT>
  void operator()(IteratorT first, IteratorT last) const
  { 
    DOUT("found include: '" << string(first, last) << "'");
  }
};

struct concat
{
  common::StringStream& target;

  concat(common::StringStream& t)
  : target(t)
  {
  }

  template<typename IteratorT>
  void operator()(IteratorT first, IteratorT last) const
  { 
    string d(first, last);
//    DOUT("CONCATENATING: '"<<d<<"'");
    target << d;
  }
};

struct resolveInclude
{
  resource::LoaderPtr loader;
  common::StringStream& target;
  uint32_t* counter;

  resolveInclude(const resource::LoaderPtr& l, common::StringStream& t, uint32_t& c)
  : loader(l), target(t)
  {
    counter = &c;
  }
  
  template<typename IteratorT>
  void operator()(IteratorT first, IteratorT last) const
  {
    string name(first, last);
//    DOUT("-- resolving include '"<<name<<"'");
    common::DataPtr includeData = loader->load(name);
    target <<"\n" << includeData->str();
    (*counter)++;
  }
};

struct resolveImport
{
  resource::LoaderPtr loader;
  common::StringStream& target;
  vector<string>* imports;
  uint32_t* counter;

  resolveImport(const resource::LoaderPtr& l, common::StringStream& t, vector<string>& i, uint32_t& c)
  : loader(l), target(t)
  {
    imports = &i;
    counter = &c;    
  }
  
  template<typename IteratorT>
  void operator()(IteratorT first, IteratorT last) const
  {
    string name(first, last);
    // check that file wasn't imported before
    bool imported = false;
    for(vector<string>::iterator i=imports->begin(); i!=imports->end(); ++i)
    {
      if(*i == name)
      {
        imported = true;
        break;
      }
    }
  
    if(!imported)
    {
//      DOUT("-- resolving import '"<<name<<"'");
      common::DataPtr importData = loader->load(name);
      target <<"\n" << importData->str();
      imports->push_back(name);
      (*counter)++;
    }
    else 
    {
//      DOUT("ignoring import '"<<name<<"', already imported once");
    }
  }
};

// resolves includes and imports
// includes are loaded and inserted every time a '#include' directive is encountered
// imports are loaded and inserted only once, and each subsequent '#import' directive is ignored 
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// FIXME: The parser will fail if the shader file does not end with a newline!
// The shader file will be incomplete and the program will terminate due to 
// and uncompilable broken file.
// I simply didn't know how to prevent spirit from matching endlessly upon 
// encountering end_p :(
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
string processDependencies(const resource::LoaderPtr& loader, const string& source)
{
  lost::common::StringStream os;
  string buffer = source;
  uint32_t counter;
  vector<string> imports;

  rule<> unknown_p = *(anychar_p - eol_p) >> eol_p;
  rule<> include = *space_p >> 
                ch_p('#') >> 
                *space_p >>
                str_p("include") >> 
                *space_p >> 
                ch_p('\"') >> 
                (*(anychar_p-ch_p('\"')))[resolveInclude(loader, os, counter)] >> 
                ch_p('\"') >> 
                unknown_p;
  rule<> import = *space_p >> 
                ch_p('#') >> 
                *space_p >>
                str_p("import") >> 
                *space_p >> 
                ch_p('\"') >> 
                (*(anychar_p-ch_p('\"')))[resolveImport(loader, os, imports, counter)] >> 
                ch_p('\"') >> 
                unknown_p;
  rule<> line = *(include | import | unknown_p[concat(os)]);
  
  BOOST_SPIRIT_DEBUG_NODE(unknown_p);
  BOOST_SPIRIT_DEBUG_NODE(incl);
  BOOST_SPIRIT_DEBUG_NODE(line);
  do
  {
    counter = 0;
    os.clear();
    parse(buffer.c_str(), line);
    buffer = os.str();    
  }
  while(counter > 0);
  
//  DOUT("---------- SHADER AFTER DEPENDENCY PROCESSING: ");DOUT(buffer);
  
  return buffer;
}

// logs shader source with line numbers
void broken(const string& source)
{
  std::stringstream s;
  const char* cc = source.c_str();
  s << cc;
  uint32_t lineNumber = 1;
  std::string line;
  s.clear();
  while(!s.eof())
  {
    getline(s, line);
    EOUT(lineNumber << " : " << line.c_str());
    lineNumber++;
  }
}

ShaderProgramPtr buildShader(const resource::LoaderPtr& loader, const string& inName, const string& vssource, const string& fssource)
{
  // DOUT("building shader: '"<<inName<<"'");
  lost::gl::ShaderProgramPtr  shaderProgram(new lost::gl::ShaderProgram());
  lost::gl::ShaderPtr         vertexShader(new lost::gl::VertexShader());
  lost::gl::ShaderPtr         fragmentShader(new lost::gl::FragmentShader());

  string vsname = inName+".vs";
  string fsname = inName+".fs";

  string source = processDependencies(loader, vssource);
  vertexShader->source(source);
  vertexShader->compile();
  if(!vertexShader->compiled())
  {
    std::ostringstream os;
    broken(source);
    os << "vertex shader '"<<vsname<<"' compilation failed: "<<vertexShader->log();
    throw std::runtime_error(os.str());
  }

  source = processDependencies(loader, fssource);
  fragmentShader->source(source);
  fragmentShader->compile();
  if(!fragmentShader->compiled())
  {
    broken(source);
    std::ostringstream os;
    os << "fragment shader '"<<fsname<<"' compilation failed: "<<fragmentShader->log();
    throw std::runtime_error(os.str());
  }

  shaderProgram->attach(fragmentShader);
  shaderProgram->attach(vertexShader);
  shaderProgram->link();
  if(!shaderProgram->linked())
  {
    std::ostringstream os;
    os << "shader program '"<<inName<<"' link failed: "<<shaderProgram->log();
    throw std::runtime_error(os.str());
  }

  shaderProgram->enable();
  shaderProgram->buildUniformMap();
  shaderProgram->buildVertexAttributeMap();
  shaderProgram->disable();
  
  return shaderProgram;
}

ShaderProgramPtr loadShader(const resource::LoaderPtr& loader, const string& inName)
{
//  DOUT("--- loading shader '"<<inName<<"'");
  string vsname = inName+".vs";
  common::DataPtr vsfile = loader->load(vsname);
  string fsname = inName+".fs";
  common::DataPtr fsfile = loader->load(fsname);
  return buildShader(loader, inName, vsfile->str(), fsfile->str());
}

}
}