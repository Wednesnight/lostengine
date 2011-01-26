#ifndef LOST_FONT_RENDER_H
#define LOST_FONT_RENDER_H

#include "lost/font/forward.h"
#include "lost/font/Range.h"
#include <string>
#include <vector>

namespace lost
{
namespace font
{
RenderedTextPtr render(const std::string & inText, const FontPtr& font);   
RenderedTextPtr render(const fhtagn::text::utf32_string& inText, const FontPtr& font);   
void render(const std::string & inText, const FontPtr& font, const RenderedTextPtr& target);
void render(const fhtagn::text::utf32_string& inText, const FontPtr& font, const RenderedTextPtr& target);
void render(const fhtagn::text::utf32_string& inText, const Range& range, const FontPtr& font, const RenderedTextPtr& target);
void render(const fhtagn::text::utf32_string& inText, const std::vector<Range>& lines, const FontPtr& font, const RenderedTextPtr& target); 
}
}


#endif