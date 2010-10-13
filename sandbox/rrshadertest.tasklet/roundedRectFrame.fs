uniform vec4 color;
uniform vec2 size;
uniform float radius;
uniform float width;
varying vec2 tc0; 

#import "roundedRectFrame.fsp"

// returns the current fragments pixel coord in quad space
// i.e. with origin of quad = 0,0, ranged 0-(max-1)
vec2 localPixelCoord() 
{
  return tc0*(size-vec2(1,1));
}

void main(void)
{  
  gl_FragColor = color*roundedRectFrame(localPixelCoord(), size, radius, width);
}
