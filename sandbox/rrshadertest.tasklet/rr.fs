uniform vec4 color; // same semantics as gl_Color
uniform vec2 size;
uniform vec2 center;
uniform float radius;
varying vec2 vtc; 

vec4 nonAAcircle(vec2 lpc)
{
  vec4 gradColor = color;
  gradColor.r = lpc.x / size.x;
  gradColor.g = lpc.y / size.y;
  vec4 white = vec4(1,1,1,1);

  float f = length(lpc - center) - radius;
  float circleColor = 1.0 - step(radius, f);

  return (white*(circleColor)) + (gradColor*(1.0-circleColor));  
}

// returns the current fragments pixel coord in quad space
// i.e. with origin of quad = 0,0
vec2 quadPixelCoord() 
{
  return vtc*size;
}

void main(void)
{  
  
  gl_FragColor = nonAAcircle(quadPixelCoord());    
}