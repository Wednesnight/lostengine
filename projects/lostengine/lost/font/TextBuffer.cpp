#include "lost/font/TextBuffer.h"
#include <fhtagn/text/transcoding.h>
#include <fhtagn/text/decoders.h>
#include "lost/font/Font.h"
#include <boost/algorithm/string.hpp>

namespace lost
{
namespace font
{

using namespace std;
namespace ftxt = fhtagn::text;

TextBuffer::TextBuffer()
{
  _breakMode = BREAKMODE_NONE;
}

TextBuffer::~TextBuffer()
{
}

void TextBuffer::text(const std::string& inText)
{
  resetLogicalLines(inText);
}

void TextBuffer::font(const FontPtr inFont)
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

void TextBuffer::splitAtNewline(const std::string& inText)
{
  std::string txt(inText); // make a copy we can modify
  // normalise newlines so only \n is left so we can safely split newlines
  vector<string> tmp;
  boost::algorithm::ireplace_all(txt, "\r\n", "\n");
  boost::algorithm::ireplace_all(txt, "\r", "\n");
  boost::algorithm::split(tmp, txt, boost::algorithm::is_any_of("\n"));

  vector<string>::iterator pos;
//  DOUT("split ---------------");
  ftxt::utf8_decoder decoder;
  for(pos=tmp.begin(); pos!=tmp.end(); ++pos)
  {
//    DOUT("'"<<*pos<<"'");

    Utf32String decodedString;
    ftxt::decode(decoder, (*pos).begin(), (*pos).end(),
           std::back_insert_iterator<Utf32String>(decodedString));
    LogicalLinePtr ll(new LogicalLine);
    ll->line.reset(new Utf32String(decodedString));
    _logicalLines.push_back(ll);
  }
}

void TextBuffer::resetLogicalLines(const std::string& inText)
{
  _logicalLines.clear();
  splitAtNewline(inText);
  resetPhysicalLines();
}

void TextBuffer::resetPhysicalLines()
{
}

}
}