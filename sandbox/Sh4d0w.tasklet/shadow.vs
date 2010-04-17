uniform mat4 textureMatrix;  // texture transform
uniform mat4 modelViewMatrix;  // mesh transform
uniform mat4 projectionMatrix; // from camera
uniform vec4 color; // same semantics as gl_Color

attribute vec3 position;

varying vec4 shadowTexCoord;

void main(void)
{
  vec4 pos = vec4(position, 1.0);
  gl_Position = projectionMatrix*modelViewMatrix*pos; // equivalent to builtin function ftransform()
  shadowTexCoord = textureMatrix * modelViewMatrix * pos;
}
