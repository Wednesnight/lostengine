#ifndef LOST_FONT_RENDER_H
#define LOST_FONT_RENDER_H

#include "lost/font/forward.h"
#include <string>

namespace lost
{
namespace font
{
RenderedTextPtr render(const std::string & inText, const FontPtr& font);   
void render(const std::string & inText, const FontPtr& font, const RenderedTextPtr& target);
}
}


#endif