#ifndef LOST_FONT_ENGINE_H
#define LOST_FONT_ENGINE_H

#include "lost/forward/boost/shared_ptr.hpp"
#include <string>
#include "lost/font/freetype/Face.h"
#include "lost/font/Model.h"
#include <map>

namespace lost
{
  
  namespace bitmap { struct Bitmap;}  
namespace font
{
  namespace freetype { struct face; }
  
/** This is a helper class for the LostEngine font system that manages fonts
 * and renders them. You can register fonts under an alias and use them for subsequent 
 * operations.
 *
 * You don't necessarily have to use this class to use the font system, but it makes life 
 * a lot easier. 
 */
struct Engine
{
  Engine();
  virtual ~Engine();

    
  /** lets freetype render the specified glyph.
   * All memory management is performed by freetype. The result is placed in the
   * face->glyph->bitmap structure.
   */
  static boost::shared_ptr<bitmap::Bitmap>
  renderGlyphToBitmap(boost::shared_ptr<freetype::Face> face,
                      uint32_t inSizeInPoints,
                      char c);
    
private:

  /*typedef std::string FontId;
  typedef boost::shared_ptr<freetype::Face> FreetypeFacePtr;
  typedef std::map<FontId, FreetypeFacePtr> FontCache; 
  
  FontCache fontCache;*/
};
  
}
}

#endif