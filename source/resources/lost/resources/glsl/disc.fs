uniform vec2 size;
uniform float radius;
uniform vec2 center;
uniform vec4 color; 
varying vec2 tc0; 

#import "lost/resources/glsl/disc.fsp"

vec2 localPixelCoord() 
{
  return tc0*(size-vec2(1,1));
}

void main(void)
{  
  gl_FragColor = color*disc(localPixelCoord(), center, radius);    
}
