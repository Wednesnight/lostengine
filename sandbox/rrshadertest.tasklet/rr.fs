uniform vec4 color; // same semantics as gl_Color
uniform vec2 size;
uniform vec2 center;
uniform float radius;
varying vec2 vtc; 

float disc(vec2 lpc, vec2 c, float r)
{
  vec4 white = vec4(1,1,1,1);

  float dist = distance(lpc ,c);
  float circleColor = 1.0 - step(r, dist);
  float aawidth = 1.0;
  float aa = (step(r-aawidth, dist)*circleColor)*fract(dist);
  circleColor = circleColor-aa;

  return circleColor; 
}

float ring(vec2 lpc, vec2 c, float r, float w)
{
  return disc(lpc, c, r) - disc(lpc, c, r-w);
}

// returns the current fragments pixel coord in quad space
// i.e. with origin of quad = 0,0, ranged 0-(max-1)
vec2 localPixelCoord() 
{
  return vtc*(size-vec2(1,1));
}

void main(void)
{  
  float f = 0.0;
  f = disc(localPixelCoord(), center, radius);    
//  f = ring(localPixelCoord(), center, radius, 6.0);
  gl_FragColor = color*f;
}
