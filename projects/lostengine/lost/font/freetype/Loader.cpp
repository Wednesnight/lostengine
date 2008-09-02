#include "lost/font/freetype/Loader.h"

namespace lost
{
namespace font
{
namespace freetype
{

Loader::Loader(boost::shared_ptr<lost::font::freetype::Library> inLibrary,
       boost::shared_ptr<lost::resource::Loader> inResourceLoader)
: library(inLibrary), resourceLoader(inResourceLoader)
{
}

Loader::~Loader()
{
}

boost::shared_ptr<Face> Loader::load(const boost::filesystem::path& inPath)
{
  boost::shared_ptr<lost::resource::File> file = resourceLoader->load(inPath);
  return library->initFace(file);
}
  
}
}
}