uniform vec4 color;
uniform vec2 size; 
varying vec2 tc0; 

// returns 1 if p is inside the box described by the two points bl (bottom left) and tr (top right)
float box(vec2 p, vec2 bl, vec2 tr)
{
  bvec2 t1 = bvec2(step(bl, p));
  bvec2 t2 = bvec2(vec2(1,1) - step(tr, p));
  return float(all(equal(equal(bvec2(true,true), t1), t2)));
}

float box2(vec2 p, vec2 pos, vec2 size)
{
  vec2 p2 = abs(p-pos)-(size);
  float f = max(p2.x, p2.y);
  return clamp(1.0-f, 0.0, 1.0);
}

vec2 localPixelCoord() 
{
  return tc0*(size-vec2(0,0));
}

void main(void)
{ 
  float foff = 2.0;
  vec2 off = vec2(foff,foff);
  float f = box2(localPixelCoord(), size*0.5, size*0.5-off);
  gl_FragColor = color*f;
}
