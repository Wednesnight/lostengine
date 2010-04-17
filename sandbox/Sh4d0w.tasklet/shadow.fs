uniform vec4 color; // same semantics as gl_Color
uniform sampler2DShadow texture0;

varying vec4 shadowTexCoord;

void main(void)
{
  gl_FragColor = shadow2DProj(texture0, shadowTexCoord).r * color;
}
