uniform vec3 lightPosition;
uniform mat4 textureMatrix;  // texture transform
uniform mat4 modelViewMatrix;  // mesh transform
uniform mat4 projectionMatrix; // from camera

attribute vec3 position;
attribute vec3 normal;

varying float lightIntensity;  // will becalculated per vertex and interpolated per fragment
varying vec4 shadowTexCoord;

void main(void)
{
  vec4 pos = vec4(position, 1.0);
  gl_Position = projectionMatrix*modelViewMatrix*pos; // equivalent to builtin function ftransform()

	vec3 ecpos = vec3(modelViewMatrix * pos);
	vec3 lightVec = normalize(lightPosition-ecpos);
  vec3 tnorm = vec3(normalize(modelViewMatrix*vec4(normal,0.0)));
  lightIntensity = max(dot(lightVec, tnorm), 0.0);
  shadowTexCoord = textureMatrix * modelViewMatrix * pos;
}
