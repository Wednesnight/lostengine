#ifndef LOST_GUIRO_CONTROL_IMAGE_H
#define LOST_GUIRO_CONTROL_IMAGE_H

#include <boost/shared_ptr.hpp>

#include "lost/guiro/control/View.h"
#include "lost/bitmap/Bitmap.h"
#include "lost/gl/Scissor.h"
#include "lost/gl/gl.h"

#include "lost/object/ObjectReference.h"
#include "lost/object/ObjectReferenceResolver.h"

#include "lost/common/Logger.h"

namespace lost
{
  namespace guiro
  {
    namespace control
    {

    struct Image : public View
    {
      object::ObjectReference<lost::bitmap::Bitmap> bitmap;

      Image( const std::string& inName ) : View(inName) {}

      Image( Image& inImage )
        : View(inImage),
          bitmap(inImage.bitmap)
      {
      }

      virtual ~Image() {}

      virtual Image* clone()
      {
        return new Image(*this);
      }

      virtual void draw(const lost::math::Rect& parentRect, const lost::common::Color& parentColor )
      {
        if (getVisible())
        {
          lost::math::Rect clippedRect = rect;
          clippedRect.clipTo( parentRect );

          lost::common::Color currentColor = color * parentColor;
          lost::gl::Scissor scissor( rect );

          glColor4fv( currentColor.fv );
          bitmap->render( clippedRect.x, clippedRect.y );
          View::draw(clippedRect, parentColor );
        }
      }

      virtual void resolve( object::ObjectReferenceResolver& inResolver )
      {
        bitmap.resolve( inResolver );
        View::resolve( inResolver );
      }
    };
    }
  }
}

#endif
