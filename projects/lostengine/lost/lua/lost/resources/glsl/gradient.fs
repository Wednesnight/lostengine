varying vec2 vtexcoord0;
varying vec2 vtexcoord1;
uniform vec4 color; // same semantics as gl_Color
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float gradientCoord; // horizontal gradient texture coordinate in texture1

void main(void)
{
  vec4 t0col = texture2D(texture0, vtexcoord0);
  vec4 gradientCol = texture2D(texture1, vec2(gradientCoord, vtexcoord1.y));
  gl_FragColor = color*t0col*gradientCol;
}