uniform vec4 color;
uniform vec2 size;
uniform float radius;
varying vec2 tc0; 

#import "lost/resources/glsl/roundedRect.fsp"

// returns the current fragments pixel coord in quad space
// i.e. with origin of quad = 0,0, ranged 0-(max-1)
vec2 localPixelCoord() 
{
  return tc0*(size-vec2(1,1));
}

void main(void)
{ 
  float f = roundedRect(localPixelCoord(), size, radius);
  gl_FragColor = color*f;
}
