// constant matrix used to scale shadowTexCoord to texture coordinates
uniform mat4 biasMatrix;

// from light cam
uniform vec3 lightPosition;
uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

// mesh transform
uniform mat4 modelViewMatrix;

// from camera
uniform mat4 projectionMatrix;

attribute vec3 position;
attribute vec3 normal;

// will be calculated per vertex and interpolated per fragment
varying float lightIntensity;

// shadow map params
varying vec4 shadowTexCoord;

void main(void)
{
  /*
   * position calculation
   */
  vec4 pos = vec4(position, 1.0);
  gl_Position = projectionMatrix*modelViewMatrix*pos; // equivalent to builtin function ftransform()

  /*
   * light shader params
   */
	vec3 ecpos = vec3(modelViewMatrix * pos);
	vec3 lightVec = normalize(lightPosition-ecpos);
  vec3 tnorm = vec3(normalize(modelViewMatrix*vec4(normal,0.0)));
  lightIntensity = max(dot(lightVec, tnorm), 0.0);

  /*
   * shadow projection
   */
  shadowTexCoord = biasMatrix * lightProjectionMatrix * lightViewMatrix * modelViewMatrix * pos;
}
