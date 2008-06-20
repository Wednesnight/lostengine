#ifndef LOST_GUIRO_CONTROL_PROGRESSBAR_H
#define LOST_GUIRO_CONTROL_PROGRESSBAR_H

#include "lost/guiro/control/View.h"
#include "lost/gl/Scissor.h"

namespace lost
{
  namespace guiro
  {
    namespace control
    {
    struct ProgressBar : public View
    {
      int min;
      int max;
      int stepSize;
      int position;

      ProgressBar( const std::string& inName )
      : View(inName),
        min(0),
        max(100),
        stepSize(1),
        position(0)
      {
      }

      virtual ~ProgressBar() {}

      virtual void draw( lost::math::Rect& parentRect, lost::common::Color& parentColor )
      {
        if (getVisible())
        {
          lost::math::Rect clippedRect = rect;
          clippedRect += parentRect.pos();
          clippedRect.clipTo( parentRect );

          lost::common::Color       borderColor  = lost::common::blackColor * parentColor;
          lost::common::Color       contentColor = color * parentColor;
          lost::gl::Scissor scissor( clippedRect );

          glColor4fv( borderColor.fv );
          glBegin(GL_LINE_LOOP);
            glVertex2f( clippedRect.x, clippedRect.y );
            glVertex2f( clippedRect.x + clippedRect.width - 1, clippedRect.y );
            glVertex2f( clippedRect.x + clippedRect.width - 1, clippedRect.y + clippedRect.height - 1 );
            glVertex2f( clippedRect.x, clippedRect.y + clippedRect.height - 1 );
          glEnd();

          float width = (clippedRect.width / (max - min)) * position;
          if (width > 0)
          {
            glColor4fv( contentColor.fv );
            glBegin(GL_QUADS);
              glVertex2f( clippedRect.x + 1, clippedRect.y + 1 );
              glVertex2f( clippedRect.x + width - 2, clippedRect.y + 1 );
              glVertex2f( clippedRect.x + width - 2, clippedRect.y + clippedRect.height - 1 );
              glVertex2f( clippedRect.x + 1, clippedRect.y + clippedRect.height - 1 );
            glEnd();
          }
        }
        View::draw( parentRect, parentColor );
      }

      void step()
      {
        (position < min || position >= max) ? position = min : position += stepSize;
      }

    };
    }
  }
}

#endif
