#ifndef LOST_FONT_FREETYPE_RENDERER_H
#define LOST_FONT_FREETYPE_RENDERER_H

#include "lost/font/freetype/Face.h"
#include <boost/shared_ptr.hpp>
#include "lost/gl/Texture.h"
#include <string>
#include "lost/gl/DisplayList.h"
#include "lost/font/freetype/Glyph.h"
#include <map>
#include "lost/math/Rect.h"

namespace lost
{
namespace font
{
namespace freetype
{

struct Renderer
{

  Renderer();  
  virtual ~Renderer();

  /** helper function for quick rendering of a string into a dispay list.
   * This is not really meant for anything else but short labels or one-liners.
   *
   * @param inSizeInPoints    size of text in points
   * @param inText            the text to render
   * @param outList           an existing list that's overwritten with the new content
   */
  void renderText(boost::shared_ptr<Face> face,
                  unsigned long inSizeInPoints,
                  const std::string& inText,
                  boost::shared_ptr<lost::gl::DisplayList>& outList);

private:
  /** lets freetype render the specified glyph.
   * All memory management is performed by freetype. The result is placed in the
   * face->glyph->bitmap structure.
   */
  void renderGlyphToBitmap(boost::shared_ptr<Face> face,
                           unsigned long inSizeInPoints,
                           char c);

  /** renders the provided character to a bitmap and converts this bitmap into a opengl texture.
   * Depending on the settings, the texture will be forced to a power-of-two texture and potentially use more
   * memory, but provide better backwards compatibility.
   *
   * @param inCharacter   the character to render
   *
   * @return outTexture    returns a new texture object containing the converted bitmap data
   */
  // NOTE: set face size before rendering!
  boost::shared_ptr<lost::gl::Texture> renderGlyphToTexture(boost::shared_ptr<Face> face,
                            unsigned long inSizeInPoints,
                            char inCharacter);

  /** always renders and creates a new glyph object.
   * if you want to us ethe cache, use getGlyph(...);
   *
   */
  boost::shared_ptr<lost::font::freetype::Glyph> renderGlyph(boost::shared_ptr<Face> face,
                                                             unsigned long inSizeInPoints,
                                                             char inCharacter);

  /** performs a cache lookup for the requested glyph and returns the previously build variant or
   * renders a new one, caches and returns it.
   */
  boost::shared_ptr<lost::font::freetype::Glyph> getGlyph(boost::shared_ptr<Face> face,
                                                          unsigned long inSizeInPoints,
                                                          char inCharacter);

  typedef boost::shared_ptr<lost::font::freetype::Glyph> GlyphPtr;
  typedef std::map<char, GlyphPtr>                       CharToGlyphMap;
  typedef std::map<unsigned long, CharToGlyphMap >       GlyphCache;

  GlyphCache  glyphCache;  
};

}
}
}

#endif

