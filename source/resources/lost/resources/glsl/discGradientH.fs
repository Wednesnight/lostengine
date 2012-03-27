uniform vec2 size;
uniform vec2 center;
uniform vec4 color; 
uniform float radius;
uniform sampler2D texture0;
uniform float gradientCoord;
varying vec2 tc0; 

#import "lost/resources/glsl/disc.fsp"
#import "lost/resources/glsl/gradient.fsp"

vec2 localPixelCoord() 
{
  return tc0*(size-vec2(1,1));
}

void main(void)
{  
  gl_FragColor = color*disc(localPixelCoord(), center, radius)*gradientH(tc0, gradientCoord, texture0);    
}
