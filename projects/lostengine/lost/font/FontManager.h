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
  
  void addEntry(const string& name, resource::BundlePtr bundle, const string& pathToData);
  FontPtr getFont(const string& name, uint32_t size);
  void logStats();
  
  freetype::LibraryPtr lib();
  map<string, string> name2path;
  map<string, resource::BundlePtr> name2bundle;
  map<string, lost::common::DataPtr> path2data;
  map<pair<string, uint32_t>, FontPtr> nameAndSize2font;
  
  freetype::LibraryPtr  _lib;
  resource::LoaderPtr   _loader;
};
}
}

#endif
