#ifndef LOST_FONT_ENGINE_H
#define LOST_FONT_ENGINE_H

#include "lost/forward/boost/shared_ptr.hpp"
#include <string>
#include "lost/font/freetype/Face.h"
#include "lost/font/Model.h"
#include <map>

namespace lost
{
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
  
  /** adds the given freetype font under the provided alias.
   *
   * @param inId id for the given font.
   * @param inFtFace fuly initialised freety font face object.
   *
   * @throws runtime_error if the font id was already used.
   *
   */
  void addFont(const std::string& inId,
               boost::shared_ptr<freetype::Face> inFtFace);
  
  /** tries to find the font for the given font id and renders the provided text into 
   * a model.
   * The resulting model holds a textured mesh.
   */
  boost::shared_ptr<Model>
  render(const std::string& inFontId,
         const std::string& inText);
  
private:
  typedef std::string FontId;
  typedef boost::shared_ptr<freetype::Face> FreetypeFacePtr;
  typedef std::map<FontId, FreetypeFacePtr> FontCache; 
  
  FontCache fontCache;
};
  
}
}

#endif