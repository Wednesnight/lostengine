uniform vec4 color;
uniform vec2 size;
varying vec2 tc0; 

#import "box.fsp"

vec2 localPixelCoord() 
{
  return tc0*(size-vec2(0,0));
}

void main(void)
{ 
  gl_FragColor = color*box2(localPixelCoord(), vec2(0.,0.), vec2(1., 4.));
}
