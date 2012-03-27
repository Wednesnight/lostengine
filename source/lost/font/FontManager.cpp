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

#include "lost/font/FontManager.h"
#include "lost/font/freetype/Library.h"
#include "lost/common/Logger.h"
#include "lost/common/Data.h"
#include "lost/resource/Bundle.h"
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

void FontManager::addEntry(const string& name, resource::BundlePtr bundle, const string& pathToData)
{
  name2path[name] = pathToData;
  name2bundle[name] = bundle;
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
        resource::BundlePtr bundle = name2bundle[name];
        data = bundle->load(path);
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
