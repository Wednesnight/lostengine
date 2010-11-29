uniform vec4 color;
varying float lightIntensity;  // will becalculated per vertex and interpolated per fragment

void main(void)
{ 
  gl_FragColor = color*lightIntensity;
}
