uniform vec3 lightPosition;
varying float lightIntensity;

void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
	gl_Position = ftransform();

	vec3 ecpos = vec3(gl_ModelViewMatrix * gl_Vertex);
	vec3 lightVec = normalize(lightPosition-ecpos);
	vec3 tnorm = normalize(gl_NormalMatrix*gl_Normal);
	lightIntensity = max(dot(lightVec, tnorm), 0.0);
}
