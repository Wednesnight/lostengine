#import "lost/resources/glsl/lftransform.vsp"

attribute vec2 position;
attribute vec2 texcoord0;

varying vec2 tc0; 

void main(void)
{
  tc0 = texcoord0;
  gl_Position = lftransform(vec4(position,0.0, 1.0));
}

