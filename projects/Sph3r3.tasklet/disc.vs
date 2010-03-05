uniform vec3 lightPosition;
varying float lightIntensity;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
  gl_Position = projectionMatrix*modelViewMatrix*gl_Vertex;

	vec3 ecpos = vec3(modelViewMatrix * gl_Vertex);
	vec3 lightVec = normalize(lightPosition-ecpos);
  vec3 tnorm = vec3(normalize(modelViewMatrix*vec4(gl_Normal,0.0)));
  lightIntensity = max(dot(lightVec, tnorm), 0.0);
}
