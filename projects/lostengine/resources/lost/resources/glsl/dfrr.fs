uniform vec4 color;
uniform vec2 size;
uniform float radius;
varying vec2 tc0; 
uniform float width;

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

void main(void)
{ 
  vec2 pos = size*.5-vec2(.5);
  vec2 rrsize = size-vec2(1);
//float  f = roundedRect(localPixelCoord(), pos, rrsize, radius);
  float f =  roundedRectFrame(localPixelCoord(), width, pos, rrsize, radius);
  gl_FragColor = color*f;
}
