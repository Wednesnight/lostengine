#include "lost/font/FontManager.h"

namespace lost
{
namespace font
{

FontManager::FontManager()
{
}

FontManager::~FontManager()
{
}

FontManagerPtr FontManager::create()
{
  return FontManagerPtr(new FontManager);
}

}
}