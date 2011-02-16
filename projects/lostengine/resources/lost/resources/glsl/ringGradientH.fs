uniform vec2 size;
uniform vec2 center;
uniform vec4 color; 
varying vec2 tc0; 
uniform sampler2D texture0; // gradient
uniform float radius;
uniform float width;
uniform float gradientCoord;

#import "lost/resources/glsl/ring.fsp"
#import "lost/resources/glsl/gradient.fsp"

vec2 localPixelCoord() 
{
  return tc0*(size-vec2(1,1));
}

void main(void)
{  
  gl_FragColor = color*ring(localPixelCoord(), center, radius, width)*gradientH(tc0, gradientCoord, texture0);    
}
