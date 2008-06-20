uniform float CoolestTemp;
uniform float TempRange;

attribute float VertexTemp;

varying float Temperature;


void main()
{
  Temperature = (VertexTemp - CoolestTemp) / TempRange;
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}