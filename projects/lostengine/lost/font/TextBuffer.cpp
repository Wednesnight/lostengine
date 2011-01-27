#include "lost/font/TextBuffer.h"
#include <fhtagn/text/transcoding.h>
#include <fhtagn/text/decoders.h>
#include "lost/font/Font.h"
#include <boost/algorithm/string.hpp>
//#include "lost/common/Logger.h"
#include "lost/font/Render.h"
#include "lost/font/Glyph.h"

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

void TextBuffer::reset(const std::string& utf8String,
                       const FontPtr& inFont,
                       BreakMode inBreakMode,
                       float width)
{
  _font = inFont;
  _width = width;
  _breakMode = inBreakMode;
  resetLogicalLines(utf8String);  
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
  _text.clear();
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
    else {
      // must check for empty line
      if(nextPos < _text.size())
      {
        _logicalLines.push_back(LogicalLine(Range(pos, pos)));
        ++nextPos;
      }
    }
  } while (pos != nextPos);
  
  resetPhysicalLines();
}

void TextBuffer::breakModeNone()
{
  for(std::vector<LogicalLine>::iterator pos=_logicalLines.begin(); pos!=_logicalLines.end(); ++pos)
  {
    _physicalLines.push_back(pos->line);
  }
}

void TextBuffer::breakModeChar()
{
  for(std::vector<LogicalLine>::iterator line=_logicalLines.begin(); line!=_logicalLines.end(); ++line)
  {
    Range& lr = line->line;
    if(lr.begin == lr.end) // skip empty lines
    {
      _physicalLines.push_back(lr); // doesn't matter what we push here as long as begin == end
      continue;
    }
    uint32_t prevChar = 0;
    float sum = 0.0f;
    Range curRange;
    for(uint32_t i=lr.begin; i<lr.end; ++i)
    {
      uint32_t curChar = _text[i];
      float adv = 0.0f;
      GlyphPtr glyph = _font->glyph(curChar);
      if(glyph) { adv = _font->kerningOffset(prevChar, curChar) + glyph->advance; }
      prevChar = curChar;
      if((sum+adv)<_width)
      {
        curRange.end++;
        sum+=adv;
        if(i == lr.end-1)
        {
          _physicalLines.push_back(Range(lr.begin+curRange.begin, lr.begin+curRange.end));
        }
      }
      else
      {
        _physicalLines.push_back(Range(lr.begin+curRange.begin, lr.begin+curRange.end));
        curRange.begin = curRange.end;
        curRange.end++;
        prevChar = 0;
        if(glyph) { sum = _font->kerningOffset(prevChar, curChar) + glyph->advance; }
        else
          sum = 0.0f;
        // if the trigger was the last character, we have to add it since the loop will now finish and it would be dropped
        if(i == lr.end-1)
        {
          _physicalLines.push_back(Range(lr.begin+curRange.begin, lr.begin+curRange.end));            
        }
      }
    }
  }
}

bool isWhiteSpace(uint32_t c)
{
  bool result = false;
  switch(c)
  {
    case ' ':
    case '\t':
    case '\n':
      result=true;
      break;
    default:
      result=false;
  }
  return result;
}

void TextBuffer::breakModeWord()
{
  for(std::vector<LogicalLine>::iterator line=_logicalLines.begin(); line!=_logicalLines.end(); ++line)
  {
    Range& lr = line->line;
    if(lr.begin == lr.end) // skip empty lines
    {
      _physicalLines.push_back(lr); // doesn't matter what we push here as long as begin == end
      continue;
    }
    for(uint32_t i=lr.begin; i<lr.end; ++i)
    {
    }
  }
}

void TextBuffer::resetPhysicalLines()
{
  _physicalLines.clear();
  switch(_breakMode)
  { 
    case BREAKMODE_NONE:breakModeNone();break;
    case BREAKMODE_CHAR:breakModeChar();break;
    case BREAKMODE_WORD:breakModeWord();break;
  }
}

void TextBuffer::renderPhysicalLine(uint32_t num, const RenderedTextPtr& target)
{
  render(_text, _physicalLines[num], _font, target);
}

void TextBuffer::renderAllPhysicalLines(const RenderedTextPtr& target)
{
  render(_text, _physicalLines, _font, target);
}

}
}