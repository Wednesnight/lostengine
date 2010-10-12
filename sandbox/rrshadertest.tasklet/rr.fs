uniform vec4 color; // same semantics as gl_Color
uniform vec2 size;
uniform vec2 center;
uniform float radius;
varying vec2 vtc; 

#import "disc.fsp"
#import "ring.fsp"

// returns the current fragments pixel coord in quad space
// i.e. with origin of quad = 0,0, ranged 0-(max-1)
vec2 localPixelCoord() 
{
  return vtc*(size-vec2(1,1));
}

void main(void)
{  
  float f = 0.0;
//  f = disc(localPixelCoord(), center, radius);    
  f = ring(localPixelCoord(), center, radius, 6.0);
  gl_FragColor = color*f;
}
