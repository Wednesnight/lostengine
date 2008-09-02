#ifndef LOST_FONT_FREETYPE_H
#define LOST_FONT_FREETYPE_H

#include "lost/resource/Loader.h"
#include "lost/font/freetype/Library.h"
#include "lost/font/freetype/Face.h"
#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>

namespace lost
{
namespace font
{
namespace freetype
{

struct Loader
{
  Loader(boost::shared_ptr<Library> inLibrary,
         boost::shared_ptr<resource::Loader> inResourceLoader);
  virtual ~Loader();
  
  boost::shared_ptr<Face> load(const boost::filesystem::path& inPath);
  
private:
  boost::shared_ptr<resource::Loader>         resourceLoader;
  boost::shared_ptr<Library>  library;
};

}
}
}

#endif