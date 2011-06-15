#ifndef LOST_FONT_RENDER_H
#define LOST_FONT_RENDER_H

#include "lost/font/forward.h"
#include "lost/font/Range.h"

namespace lost
{
namespace font
{
RenderedTextPtr render(const string & inText, const FontPtr& font, bool characterMetrics, int align = 0);
RenderedTextPtr render(const fhtagn::text::utf32_string& inText, const FontPtr& font, bool characterMetrics, int align = 0);
void render(const string & inText, const FontPtr& font, const RenderedTextPtr& target, bool characterMetrics, int align = 0);
void render(const fhtagn::text::utf32_string& inText, const FontPtr& font, const RenderedTextPtr& target, bool characterMetrics, int align = 0);
void render(const fhtagn::text::utf32_string& inText, const Range& range, const FontPtr& font, const RenderedTextPtr& target, bool characterMetrics, int align = 0);
void render(const fhtagn::text::utf32_string& inText, const vector<Range>& lines, const FontPtr& font, const RenderedTextPtr& target, bool characterMetrics, int align = 0);
}
}


#endif