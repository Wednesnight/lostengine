#ifndef LOST_GUIRO_CONTROL_QUAD_H
#define LOST_GUIRO_CONTROL_QUAD_H

#include <boost/shared_ptr.hpp>

#include "lost/common/Logger.h"
#include "lost/guiro/control/View.h"
#include "lost/guiro/common/Texture.h"

namespace lost
{
  namespace guiro
  {
    namespace control
    {
    struct Quad : public View
    {
      object::ObjectReference<lost::guiro::common::Texture> texture;
      lost::math::Vec2 bl;
      lost::math::Vec2 br;
      lost::math::Vec2 tl;
      lost::math::Vec2 tr;
      GLenum sfactor;
      GLenum dfactor;

      Quad( const std::string& inName )
      : View(inName)
      {
        // FIXME: make these serialisable
        sfactor = GL_SRC_ALPHA;
        dfactor = GL_ONE_MINUS_SRC_ALPHA;
        float w = 1.0f;
        float h = 1.0f;
        bl = lost::math::Vec2(0.0f, h);
        br = lost::math::Vec2(w, h);
        tr = lost::math::Vec2(w, 0);
        tl = lost::math::Vec2(0.0f, 0);

      }

      Quad( Quad& inQuad )
        : View(inQuad),
          texture(inQuad.texture),
          bl(inQuad.bl),
          br(inQuad.br),
          tl(inQuad.tl),
          tr(inQuad.tr),
          sfactor(GL_SRC_ALPHA),
          dfactor(GL_ONE_MINUS_SRC_ALPHA)
      {
      }

      virtual ~Quad() {}

      virtual Quad* clone()
      {
        return new Quad(*this);
      }

      virtual void draw(const lost::math::Rect& parentRect, const lost::common::Color& parentColor )
      {
        if (getVisible())
        {
          lost::math::Rect clippedRect = rect;
          clippedRect.clipTo( parentRect );

          // FIXME: I changed the default view color back to white, we can probably reenable the color multiplication again
          lost::common::Color currentColor = color * parentColor;
          lost::gl::Scissor scissor( clippedRect );

          // FIXME: push/pop attribs, don't just change a seemingly global state
          glColor4fv( currentColor.fv );
          glEnable(GL_BLEND);
          glBlendFunc(sfactor, dfactor);
          // FIXME: optionally enable texture if present. Otherwise, just draw a quad with the desired color
          glEnable(GL_TEXTURE_2D);
          glBindTexture(GL_TEXTURE_2D, texture->glTexture);

          // FIXME: do we really need the width/height negative checks? I simply copied this from the old guiro Quad implementation
          float x = clippedRect.x;
          float y = clippedRect.y;
          float width = clippedRect.width;
          if(width < 0.0f) width = 0.0f;
          float height = clippedRect.height;
          if(height < 0.0f) height = 0.0f;
          glBegin(GL_QUADS);
          glTexCoord2f(bl.x,bl.y);glVertex2f(x, y);
          glTexCoord2f(br.x,br.y);glVertex2f(x+width, y);
          glTexCoord2f(tr.x,tr.y);glVertex2f(x+width, y+height);
          glTexCoord2f(tl.x,tl.y);glVertex2f(x, y+height);
          glEnd();
          glDisable(GL_TEXTURE_2D);
          View::draw(clippedRect, parentColor );
        }
      };

      virtual void resolve( object::ObjectReferenceResolver& inResolver )
      {
        texture.resolve( inResolver );
        View::resolve( inResolver );
      }
    };
  }
  }
}

#endif
