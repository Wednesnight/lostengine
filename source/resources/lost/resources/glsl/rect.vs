#import "lost/resources/glsl/lftransform.vsp"

attribute vec2 position;

void main(void)
{
  gl_Position = lftransform(vec4(position,0.0, 1.0)); 
}
