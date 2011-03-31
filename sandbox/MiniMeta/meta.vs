uniform mat4 modelViewMatrix;  // mesh transform
uniform mat4 projectionMatrix; // from camera
uniform vec4 color; // same semantics as gl_Color

attribute vec3 position;
attribute vec3 vertexAttrib0;

varying vec3 va0;

void main(void)
{
  vec4 pos = vec4(position, 1.0);
  va0 = vertexAttrib0;
  gl_Position = projectionMatrix*modelViewMatrix*pos; // equivalent to builtin function ftransform()
}
