uniform mat4 modelViewMatrix;  // mesh transform
uniform mat4 projectionMatrix; // from camera
uniform vec4 color; // same semantics as gl_Color

attribute vec3 position;
attribute vec2 texcoord0;
attribute vec2 texcoord1;

varying vec2 vtexcoord0;
varying vec2 vtexcoord1;

void main(void)
{
  vec4 pos = vec4(position, 1.0);
  vtexcoord0 = texcoord0;
  vtexcoord1 = texcoord1;
  gl_Position = projectionMatrix*modelViewMatrix*pos; // equivalent to builtin function ftransform()
}
