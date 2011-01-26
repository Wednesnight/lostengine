#include "lost/font/TextBuffer.h"
#include <fhtagn/text/transcoding.h>
#include <fhtagn/text/decoders.h>
#include "lost/font/Font.h"
#include <boost/algorithm/string.hpp>
#include "lost/common/Logger.h"
#include "lost/font/Render.h"

namespace lost
{
namespace font
{

using namespace std;
namespace ftxt = fhtagn::text;

TextBuffer::TextBuffer()
{
  _breakMode = BREAKMODE_NONE;
  DOUT("");
}

TextBuffer::~TextBuffer()
{
  DOUT("");
}

void TextBuffer::text(const std::string& inText)
{
  resetLogicalLines(inText);
}

void TextBuffer::font(const FontPtr& inFont)
{
  _font = inFont;
  resetPhysicalLines();
}

void TextBuffer::breakMode(BreakMode inBreakMode)
{
  _breakMode = inBreakMode;
  resetPhysicalLines();
}

uint32_t TextBuffer::numLogicalLines()
{
  return _logicalLines.size();
}

uint32_t TextBuffer::numPhysicalLines()
{
  return _physicalLines.size();
}

void TextBuffer::normaliseNewlines(std::string& inText)
{
  boost::algorithm::ireplace_all(inText, "\r\n", "\n");
  boost::algorithm::ireplace_all(inText, "\r", "\n");
}

uint32_t skipToNewlineOrEnd(uint32_t pos, const lost::font::TextBuffer::Utf32String& txt)
{
  if(pos < txt.size())
  {
    while((txt[pos] != '\n') && (pos < txt.size()))
    {
      ++pos;
    }
  }
  return pos;
}

void TextBuffer::resetLogicalLines(const std::string& inText)
{
  _logicalLines.clear(); // discard all previous lines
  
  // normalise newlines into copy of text
  std::string normalised(inText); 
  normaliseNewlines(normalised);

  // transcode to utf32 for parsing
  ftxt::utf8_decoder decoder;
  ftxt::decode(decoder, normalised.begin(), normalised.end(),
         std::back_insert_iterator<Utf32String>(_text));
  
  uint32_t pos = 0;
  uint32_t nextPos = 0;
  do {
    pos = nextPos;
    nextPos = skipToNewlineOrEnd(pos, _text);
    if(nextPos != pos)
    {
      _logicalLines.push_back(LogicalLine(Range(pos, nextPos)));
      ++nextPos;
    }
  } while (pos != nextPos);
  
  resetPhysicalLines();
}

void TextBuffer::resetPhysicalLines()
{
  _physicalLines.clear();
  for(std::vector<LogicalLine>::iterator pos=_logicalLines.begin(); pos!=_logicalLines.end(); ++pos)
  {
    _physicalLines.push_back(pos->line);
  }
}

void TextBuffer::renderPhysicalLine(uint32_t num, const RenderedTextPtr& target)
{
  Range& r = _physicalLines[num];
  Utf32String s = _text.substr(r.begin, (r.end-r.begin));
  DOUT("rendering pl with size: "<<s.size());
  render(s, _font, target);
}


}
}