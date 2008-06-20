#ifndef LOST_GUIRO_CONTROL_WINDOW_H
#define LOST_GUIRO_CONTROL_WINDOW_H

#include "lost/guiro/control/View.h"
#include <boost/shared_ptr.hpp>
#include "lost/gl/Scissor.h"
#include "lost/gl/gl.h"
#include "lost/object/ObjectReferenceContainer.h"
#include "lost/object/ObjectReference.h"
#include "lost/guiro/common/Theme.h"

namespace lost
{
  namespace guiro
  {
    namespace control
    {
    struct Window : public View
    {
      boost::shared_ptr<View> titleView;
      boost::shared_ptr<View> contentView;
      lost::object::ObjectReference<lost::guiro::common::Theme> theme;

      Window( const std::string& inName ) : View(inName) {}
      virtual ~Window() {}

      virtual void resolve( object::ObjectReferenceResolver& inResolver )
      {
        theme.resolve( inResolver );
        View::resolve( inResolver );
        if(theme)
        {
          add( theme->view );
        }
        else
        {
          WOUT("theme not set: "+name);
        }
      }
      
      virtual void draw(const lost::math::Rect& parentRect, const lost::common::Color& parentColor )
      {
        if (getVisible())
        {
          lost::math::Rect clippedRect = rect;
          clippedRect.clipTo( parentRect );

          lost::common::Color       currentColor = color * parentColor;
          lost::gl::Scissor scissor( clippedRect );

          glColor4fv( currentColor.fv );
          glBegin(GL_QUADS);
            glVertex2f( clippedRect.x, clippedRect.y );
            glVertex2f( clippedRect.x + clippedRect.width, clippedRect.y );
            glVertex2f( clippedRect.x + clippedRect.width, clippedRect.y + clippedRect.height );
            glVertex2f( clippedRect.x, clippedRect.y + clippedRect.height );
          glEnd();
          View::draw(parentRect, parentColor);
        }
      }
    };
    }
  }
}

#endif
