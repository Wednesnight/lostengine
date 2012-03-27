uniform vec2 size;
uniform vec2 center;
uniform vec4 color; 
uniform float radius;
uniform float width;
varying vec2 tc0; 

#import "lost/resources/glsl/ring.fsp"

vec2 localPixelCoord() 
{
  return tc0*(size-vec2(1,1));
}

void main(void)
{  
  gl_FragColor = color*ring(localPixelCoord(), center, radius, width);    
}
