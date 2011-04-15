uniform mat4 modelViewMatrix;  // mesh transform
uniform mat4 projectionMatrix; // from camera
uniform vec4 color; // same semantics as gl_Color

attribute vec3 position;
attribute vec3 normal;
attribute vec3 vertexAttrib0;

varying vec3 va0;
varying vec4 nv;
varying vec4 pos;

void main(void)
{
  vec4 p = vec4(position, 1.0);
  va0 = vertexAttrib0;
  nv = normalize(modelViewMatrix*vec4(normal,1.0));
  pos = modelViewMatrix*p;
  gl_Position = projectionMatrix*modelViewMatrix*p;
}
