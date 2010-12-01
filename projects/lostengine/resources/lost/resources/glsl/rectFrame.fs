uniform vec4 color;
uniform vec2 size;
varying vec2 tc0; 
uniform float width;

void main(void)
{ 
  vec2 cp = tc0*size;
  float top = 1.0-step(width, size.y - cp.y);
  float bottom = 1.0-step(width, cp.y);
  float left = 1.0-step(width, cp.x);
  float right = 1.0-step(width, size.x - cp.x);
  float res = clamp(top+bottom+left+right, 0.0, 1.0);

  gl_FragColor = color*res;
}
