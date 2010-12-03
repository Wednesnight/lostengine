#ifndef LOST_FONT_FONTMANAGER_H
#define LOST_FONT_FONTMANAGER_H

#include "lost/font/forward.h"
#include "lost/font/freetype/forward.h"
#include "lost/resource/forward.h"
#include <map>
#include <string>

namespace lost
{
namespace font
{
struct FontManager
{
  FontManager(const resource::LoaderPtr& inLoader);
  ~FontManager();
  
  static FontManagerPtr create(const resource::LoaderPtr& inLoader);
  
  void addEntry(const std::string& name, const std::string& pathToData);
  TrueTypeFontPtr getFont(const std::string& name, uint32_t size);
  
  freetype::LibraryPtr lib();
  std::map<std::string, std::string> name2path;
  std::map<std::pair<std::string, uint32_t>, TrueTypeFontPtr> nameAndSize2font;
  
  freetype::LibraryPtr  _lib;
  resource::LoaderPtr   _loader;
};
}
}

#endif