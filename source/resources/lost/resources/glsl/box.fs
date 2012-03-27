uniform vec4 color;
uniform vec2 size;
varying vec2 tc0; 

#import "lost/resources/glsl/box.fsp"

vec2 localPixelCoord() 
{
  return tc0*(size-vec2(0,0));
}

void main(void)
{ 
  float off = .3;
  gl_FragColor = color*box2(localPixelCoord(), vec2(1.,3.), vec2(size.x, 1.5-off));
}
