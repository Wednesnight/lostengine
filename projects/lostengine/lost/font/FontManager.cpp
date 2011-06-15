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

void FontManager::addEntry(const string& name, const string& pathToData)
{
  name2path[name] = pathToData;
}

void FontManager::logStats()
{
  DOUT(" num fonts: "<<nameAndSize2font.size() << "num known fonts: "<<name2path.size()<<" num data chunks: "<<path2data.size());
}


FontPtr FontManager::getFont(const string& name, uint32_t size)
{
  FontPtr result;
  pair<string, uint32_t> fontKey = make_pair(name, size);
  map<pair<string, uint32_t>, FontPtr>::iterator pos = nameAndSize2font.find(fontKey);
  if(pos == nameAndSize2font.end())
  {
    map<string, string>::iterator ppos = name2path.find(name);
    if (ppos != name2path.end()) {
      string path = name2path[name];
      common::DataPtr data = path2data[path];
      if(!data)
      {
        data = _loader->load(path);
        path2data[path] = data;
      }
      result.reset(new TrueTypeFont(lib(),data, size));
      nameAndSize2font[fontKey] = result;
    }
    else {
      EOUT("no path to data registered for font name '"<<name<<"'");
    }
//    logStats();
  }
  else
  {
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