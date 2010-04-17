uniform vec4 color;
uniform sampler2DShadow texture0;

varying float lightIntensity;  // will becalculated per vertex and interpolated per fragment
varying vec4 shadowTexCoord;

void main(void)
{ 
  gl_FragColor = shadow2DProj(texture0, shadowTexCoord).r * color * lightIntensity;
}
