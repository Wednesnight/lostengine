uniform vec3 lightPosition;
varying float lightIntensity;  // will becalculated per vertex and interpolated per fragment
uniform mat4 modelViewMatrix;  // mesh transform
uniform mat4 projectionMatrix; // from camera
uniform vec4 color; // same semantics as gl_Color

attribute vec3 position;
attribute vec3 normal;
attribute vec3 texcoord0;

// gl_TexCoord, gl_FrontColor and gl_Position are built-in output variables and cannot be changed

void main(void)
{
  gl_TexCoord[0] = vec4(texcoord0, 1.0); 
	gl_FrontColor = color;
  vec4 pos = vec4(position, 1.0);
  gl_Position = projectionMatrix*modelViewMatrix*pos; // equivalent to builtin function ftransform()

	vec3 ecpos = vec3(modelViewMatrix * pos);
	vec3 lightVec = normalize(lightPosition-ecpos);
  vec3 tnorm = vec3(normalize(modelViewMatrix*vec4(normal,0.0)));
  lightIntensity = max(dot(lightVec, tnorm), 0.0);
}
