uniform mat4 modelViewMatrix;  // mesh transform
uniform mat4 projectionMatrix; // from camera
uniform vec4 color; // same semantics as gl_Color

attribute vec2 position;
attribute vec2 texcoord0;

varying vec2 vtc; 

void main(void)
{
  vec4 pos = vec4(position,0.0, 1.0);
  vtc = texcoord0;
  gl_Position = projectionMatrix*modelViewMatrix*pos; // equivalent to builtin function ftransform()
}
