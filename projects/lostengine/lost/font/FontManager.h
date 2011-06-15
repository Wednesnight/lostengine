#ifndef LOST_FONT_FONTMANAGER_H
#define LOST_FONT_FONTMANAGER_H

#include "lost/font/forward.h"
#include "lost/font/freetype/forward.h"
#include "lost/common/forward.h"
#include "lost/resource/forward.h"

namespace lost
{
namespace font
{
struct FontManager
{
  FontManager(const resource::LoaderPtr& inLoader);
  ~FontManager();
  
  static FontManagerPtr create(const resource::LoaderPtr& inLoader);
  
  void addEntry(const string& name, const string& pathToData);
  FontPtr getFont(const string& name, uint32_t size);
  void logStats();
  
  freetype::LibraryPtr lib();
  map<string, string> name2path;
  map<string, lost::common::DataPtr> path2data;
  map<pair<string, uint32_t>, FontPtr> nameAndSize2font;
  
  freetype::LibraryPtr  _lib;
  resource::LoaderPtr   _loader;
};
}
}

#endif