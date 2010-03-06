uniform vec3 lightPosition;
varying float lightIntensity;  // will becalculated per vertex and interpolated per fragment
uniform mat4 modelViewMatrix;  // mesh transform
uniform mat4 projectionMatrix; // from camera

// FIXME: the following need to move to vertex attributes
// gl_MultiTexCoord[0]
// gl_Vertex
// gl_Normal

// FIXME: the following probably can be moved to uniforms
// gl_Color

// gl_TexCoord, gl_FrontColor and gl_Position are built-in output variables and cannot be changed

void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0; 
	gl_FrontColor = gl_Color;
  gl_Position = projectionMatrix*modelViewMatrix*gl_Vertex; // equivalent to builtin function ftransform()

	vec3 ecpos = vec3(modelViewMatrix * gl_Vertex);
	vec3 lightVec = normalize(lightPosition-ecpos);
  vec3 tnorm = vec3(normalize(modelViewMatrix*vec4(gl_Normal,0.0)));
  lightIntensity = max(dot(lightVec, tnorm), 0.0);
}
