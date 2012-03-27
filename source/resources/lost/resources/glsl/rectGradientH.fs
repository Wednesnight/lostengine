uniform vec4 color; 
uniform sampler2D texture0;
uniform float gradientCoord;

#import "lost/resources/glsl/gradient.fsp"

varying vec2 tc0;

void main(void)
{
  gl_FragColor = color*gradientH(tc0, gradientCoord, texture0);
}
