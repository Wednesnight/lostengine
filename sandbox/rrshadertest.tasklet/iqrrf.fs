uniform vec4 color;
uniform vec2 size;
uniform float radius;
uniform float width;
varying vec2 tc0; 

// returns the current fragments pixel coord in quad space
// i.e. with origin of quad = 0,0, ranged 0-(max-1)
vec2 localPixelCoord() 
{
  return tc0*(size-vec2(0,0));
}

void main(void)
{ 
  float off = 0.5;
  float rad = radius +off;
  vec2 boxc = vec2((size.x-1.0)/2.0 + off,(size.y-1.0)/2.0 +off);
  vec2 offset = vec2((size.x-1.0)/2.0, (size.y-1.0)/2.0);
  float f2 = clamp(length(max(abs(localPixelCoord()-offset) - boxc + vec2(rad, rad), 0.0))-rad, 0.0, 1.0);
  f2 = 1.0-f2;

  float rad2 = clamp(rad - width - off, 0.0, rad);
  float f3 = clamp(length(max(abs(localPixelCoord()-offset) - (boxc-vec2(width,width)-vec2(off, off)) + vec2(rad2, rad2), 0.0))-rad2, 0.0, 1.0);
  f3 = 1.0-f3;

  f2 = f2 - f3;
  gl_FragColor = color*f2;
}
