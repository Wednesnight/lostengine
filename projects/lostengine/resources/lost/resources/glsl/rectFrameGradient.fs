uniform vec4 color;
uniform vec2 size;
varying vec2 tc0; 
uniform float width;
uniform sampler2D texture0;
uniform float gradientCoord;

#import "lost/resources/glsl/rectFrame.fsp"
#import "lost/resources/glsl/gradient.fsp"

void main(void)
{ 
  gl_FragColor = color*rectFrame(tc0, size, width)*gradient(tc0, gradientCoord, texture0);
}
