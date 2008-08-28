#ifndef LOST_FONT_FONT_H
#define LOST_FONT_FONT_H

namespace lost
{
  namespace font
  {

    /** base class for any 2D font thats drawn to screen without any additional transformation.
     *
     */
    struct Font 
    {
    public:
      Font();
      virtual ~Font();
      virtual void render( float x, float y ) = 0;
    };

  }
}
#endif
