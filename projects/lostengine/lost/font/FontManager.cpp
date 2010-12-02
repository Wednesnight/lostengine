#include "lost/font/FontManager.h"
#include "lost/font/freetype/Library.h"
#include "lost/common/Logger.h"
#include "lost/common/Data.h"
#include "lost/resource/Loader.h"
#include "lost/font/TrueTypeFont.h"

namespace lost
{
namespace font
{

FontManager::FontManager(const resource::LoaderPtr& inLoader)
{
  _loader = inLoader;
}

FontManager::~FontManager()
{
}

FontManagerPtr FontManager::create(const resource::LoaderPtr& inLoader)
{
  return FontManagerPtr(new FontManager(inLoader));
}

void FontManager::addEntry(const std::string& name, const std::string& pathToData)
{
  DOUT("-- ADDING FONT ENTRY: "<<name<<" : "<<pathToData);
  name2path[name] = pathToData;
}

TrueTypeFontPtr FontManager::getFont(const std::string& name, uint32_t size)
{
  TrueTypeFontPtr result;
  std::map<std::string, TrueTypeFontPtr>::iterator pos = name2font.find(name);
  if(pos == name2font.end())
  {
    std::map<std::string, std::string>::iterator ppos = name2path.find(name);
    if (ppos != name2path.end()) {
      std::string path = name2path[name];
      DOUT("loading font "<<name<<" from "<<path);
      common::DataPtr data = _loader->load(path);
      result.reset(new TrueTypeFont(lib(),data, size));
      name2font[name] = result;
      result->atlasSize = lost::math::Vec2(256,256); // FIXME: make this configurable?      
    }
    else {
      EOUT("no path to data registered for font name '"<<name<<"'");
    }
  }
  else
  {
    DOUT("returning cached font "<<name)
    result = pos->second;
  }
  return result;
}

freetype::LibraryPtr FontManager::lib()
{
  if(!_lib)
  {
    _lib = freetype::Library::create();
  }
  return _lib;
}


}
}