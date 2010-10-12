uniform vec4 color; // same semantics as gl_Color
uniform vec2 size;
//uniform vec2 center;
uniform float radius;
varying vec2 vtc; 

#import "quadrant.fsp"
#import "disc.fsp"
#import "ring.fsp"
#import "box.fsp"

// returns the current fragments pixel coord in quad space
// i.e. with origin of quad = 0,0, ranged 0-(max-1)
vec2 localPixelCoord() 
{
  return vtc*(size-vec2(1,1));
}

float rr(vec2 lpc, vec2 size, float r, float width)
{
  float minradius = min(min(size.x/2.0, size.y/2.0), r);
  float tl = quadring(lpc, vec2(minradius-1.0, size.y-r-1.0), minradius, width, bvec2(false, true));
  float tr = quadring(lpc, vec2(size.x-minradius-1.0, size.y-minradius-1.0), minradius, width, bvec2(true, true));
  float bl = quadring(lpc, vec2(minradius-1.0, minradius-1.0), minradius, width, bvec2(false, false));
  float br = quadring(lpc, vec2(size.x-minradius-1.0, minradius-1.0), minradius, width, bvec2(true, false));
  float topl = box(lpc, vec2(minradius-1.0, size.y-1.0-width), vec2(size.x-minradius-1.0, size.y));
  float botl = box(lpc, vec2(minradius-1.0, 0), vec2(size.x-minradius-1.0, width));
  float leftl = box(lpc, vec2(0, minradius-1.0), vec2(width, size.y-minradius-1.0));
  float rightl = box(lpc, vec2(size.x-width-1.0, minradius-1.0), vec2(size.x-1.0, size.y-minradius-1.0));
  return tl+tr+bl+br+topl+botl+leftl+rightl;
}

void main(void)
{  
  float f = 0.0;
//  f = disc(localPixelCoord(), center, radius);    
  vec2 lpc = localPixelCoord();
//  float a = quadrant(lpc, center, bvec2(true,true));
//  f = ring(lpc, center, radius, 1.0);
//  gl_FragColor = color*f*a;
  gl_FragColor = color*rr(lpc, size, 8.0, 2.0);
}
