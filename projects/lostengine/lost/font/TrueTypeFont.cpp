#include "lost/font/TrueTypeFont.h"

using namespace boost;
using namespace lost::font::freetype;
using namespace lost::math;

namespace lost
{
namespace font
{
    
TrueTypeFont::TrueTypeFont(boost::shared_ptr<freetype::Library> inLibrary,
             boost::shared_ptr<resource::File> inFile)
{
  face.reset(new Face(inLibrary, inFile));
}

TrueTypeFont::~TrueTypeFont()
{
}

shared_ptr<Model> TrueTypeFont::render(const std::string& inText)
{
  shared_ptr<Model>  result;
  return result;
}

}  
}
