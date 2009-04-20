varying vec3 normal;
varying vec4 vertex;

void main(void)
{
  normal = normalize(gl_NormalMatrix * gl_Normal);
  vertex = gl_ModelViewMatrix * gl_Vertex;

  gl_Position = ftransform();
  gl_FrontColor = gl_Color;
}
