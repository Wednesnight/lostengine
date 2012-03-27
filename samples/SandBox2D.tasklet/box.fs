uniform vec4 color;
uniform vec2 size; 
varying vec2 tc0; 

float box2(vec2 p, vec2 pos, vec2 size)
{
  vec2 p2 = abs(p-pos)-(size);
  float f = max(p2.x, p2.y);
  return 1.0-clamp(f, 0.0, 1.0);
}

void main(void)
{ 
  float foff = 1.0;
  vec2 off = vec2(foff,foff);
  float f = box2(tc0*size, size*0.5, size*0.5-off);
  gl_FragColor = color*f;
}
