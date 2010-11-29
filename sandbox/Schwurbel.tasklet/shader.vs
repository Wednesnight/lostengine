uniform mat4 modelViewMatrix;  // mesh transform
uniform mat4 projectionMatrix; // from camera
uniform vec4 color;
attribute vec3 position;
attribute vec2 texcoord0;
varying vec2 texcoord;

void main()
{
  vec4 pos = vec4(position, 1.0);
  gl_Position = projectionMatrix*modelViewMatrix*pos; // equivalent to builtin function ftransform()
  
	texcoord = texcoord0;
}
