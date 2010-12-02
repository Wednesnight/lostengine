uniform vec4 color;
uniform vec2 size;
varying vec2 tc0; 
uniform float width;

#import "lost/resources/glsl/rectFrame.fsp"

void main(void)
{ 
  gl_FragColor = color*rectFrame(tc0, size, width);
}
