uniform vec4 color;
varying vec2 tc0; 
uniform vec2 size;
uniform vec2 posOffset;
uniform vec2 sizeOffset;
uniform float radius;
uniform float width;
uniform float blrect;
uniform float brrect;
uniform float tlrect;
uniform float trrect;
uniform float filled;

#include "lost/resources/glsl/box.fsp"

// returns the current fragments pixel coord in quad space
// i.e. with origin of quad = 0,0, ranged 0-(max-1)
vec2 localPixelCoord() 
{
  return tc0*size;
}

// all coordinates in pixels
float roundedRect(vec2 pixel, vec2 pos, vec2 size, float radius)
{
  vec2 boxsize = size*.5;
  return 1.0-clamp(length(max(abs(pixel-pos) - boxsize + vec2(radius, radius), 0.0))-radius, 0.0, 1.0);
}

float roundedRectFrame(vec2 pixel, float width, vec2 pos, vec2 size, float radius)
{
  float outer = roundedRect(pixel, pos, size, radius);
  float inner = roundedRect(pixel, pos, size-vec2(2.0*width), max(0.0,radius-width));
  return outer - inner;
}

float hyperRoundedRect(vec2 pixel, vec2 pos, vec2 size, vec2 quarterSize, float radius, 
                       float blrect, float brrect, float tlrect, float trrect)
{
  float f = roundedRect(pixel, pos, size, radius);
  float bl = box2(pixel, pos-quarterSize, quarterSize);
  float br = box2(pixel, vec2(pos.x+quarterSize.x, pos.y-quarterSize.y), quarterSize);
  float tr = box2(pixel, vec2(pos.x+quarterSize.x, pos.y+quarterSize.y), quarterSize);
  float tl = box2(pixel, vec2(pos.x-quarterSize.x, pos.y+quarterSize.y), quarterSize);
  return clamp(f+(bl*blrect)+(br*brrect)+(tr*trrect)+(tl*tlrect), 0.0, 1.0);
}

float hyperRoundedRectFrame(vec2 pixel, float filled, float width, vec2 pos, vec2 size, vec2 quarterSize, float radius, 
                            float blrect, float brrect, float tlrect, float trrect)
{
  float outer = hyperRoundedRect(pixel, pos, size, quarterSize, radius, blrect, brrect, tlrect, trrect);
  vec2 innerSize = size-vec2(2.0*width);
  vec2 innerQuarterSize = innerSize*vec2(.25);
  float inner = hyperRoundedRect(pixel, pos, innerSize, innerQuarterSize, max(0.0,radius-width), blrect, brrect, tlrect, trrect);
  return clamp(outer - (1.0-filled)*inner, 0.0, 1.0);  
}

void main(void)
{ 
  vec2 pos = size*.5-vec2(.5)+posOffset; // must be centered in visible area, altered by posOffset
  vec2 sz = size + sizeOffset; 
  vec2 quarterSize = sz*.25; // uses modified size, NOT quad size
  vec2 rrsize = sz-vec2(1.0); // uses modified size, NOT quad size
  vec2 lp = localPixelCoord();
//  float  f = roundedRect(lp, pos, rrsize, radius);
//  float b = box2(lp, pos-q-vec2(1.0), q);
//  f = clamp(f+b, 0.0, 1.0);
//  float  f = roundedRect(lp, pos, rrsize, radius);
//  float f =  roundedRectFrame(localPixelCoord(), width, pos, rrsize, radius);
//  float f = hyperRoundedRect(lp, pos, rrsize, quarterSize, radius, 1.0);

  float f = hyperRoundedRectFrame(lp, filled, width, pos, rrsize, quarterSize, radius, blrect, brrect, tlrect, trrect);

  gl_FragColor = color*f;
}
