#ifndef LOST_FONT_GLYPH_H
#define LOST_FONT_GLYPH_H

#include "lost/math/Rect.h"
#include "lost/bitmap/forward.h"
#include "lost/font/freetype/forward.h"
namespace lost
{
namespace font
{
struct Glyph
{
    Glyph();
    ~Glyph();
    // renders a character c with given freetype face and size, initializing all values
    void render(freetype::FacePtr face, uint32_t size, uint32_t c);
    lost::math::Rect  rect; // the glyphs bitmaps rect inside the atlas texture
    
    int32_t xoffset;
    int32_t yoffset;
    int32_t advance;
    
    bool drawable;
    
    bitmap::BitmapPtr bitmap;
};
}
}

#endif