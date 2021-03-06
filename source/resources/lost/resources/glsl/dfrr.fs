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

#include "lost/resources/glsl/dfrr.fsp"

// returns the current fragments pixel coord in quad space
// i.e. with origin of quad = 0,0, ranged 0-(max-1)
vec2 localPixelCoord() 
{
  return tc0*size;
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
